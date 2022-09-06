from cmath import sqrt
import socket
import threading
from time import sleep

from helpers import buildFrames, decodeData, encodeData
from queuec import QueueC


# FRAME FORMAT:
# For a frame with length of 8, the format is:
# [sn][data][parity]
# [2 ][  8 ][  4   ]

# Ack frame format:
# [type][sn][parity]
# [  1 ][ 4 ][  4   ]
# Type -> ACK -> 1, NAK -> 0

class Sender:
    def __init__(self, host, port, timeout, N=4):
        self.host = host
        self.port = port
        self.timeout = timeout
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))
        self.sendThreadEvent = threading.Event()
        self.sendThreadEvent.clear()

        self.N = N

        self.data = QueueC()
        self.sf = 0
        self.sn = 0


        tmp = int(sqrt(N).real)
        self.snmax = 2**tmp-1

        self.sn = self.snmax

        self.ackStatus = [0]*self.N
        # 0 -> not sent
        # 1 -> sent
        # 2 -> acked

        self.resendFramesSeqNo = set()

        # Window frame status
        self.timers = [None]*self.N

        # Configure the receiver
        receiver_ip = input("Enter receiver ip: ")
        if len(receiver_ip.strip()) == 0:
            receiver_ip = "127.0.0.1"
        receiver_port = input("Enter receiver port: ")
        self.sock.sendall(str.encode(f'connect:{receiver_ip}:{receiver_port}'))
        sleep(1)

    def startProcess(self):
        self.senderThread = threading.Thread(target=self.send)
        self.receiverACKThread = threading.Thread(target=self.recvAck)
        self.receiverACKThread.start()
        self.senderThread.start()

    def send(self):
        while True:
            # Check frames
            if self.data.isEmpty():
                print("No more frames to send")
                break

            # Resend frames
            tmpResendFrames = list(self.resendFramesSeqNo)
            if len(tmpResendFrames) > 0:
                for i in tmpResendFrames:   
                    frame =  encodeData(str(bin(i)[2:]).zfill(2)+self.data.getFrameBySeqNo(seqNo=i, windowSize=self.N))
                    self.ackStatus[i] = 1
                    self.sock.sendall(str.encode(frame))
                    self.startTimer(i)
                    sleep(0.1)

            for i in tmpResendFrames:
                self.resendFramesSeqNo.remove(i)


            if (self.sn - self.sf) < self.N:
                # Prepare fram e-- by taking the sn and the element of window
                frame =  encodeData(str(bin(self.sn)[2:]).zfill(2)+self.data.getFrameBySeqNo(seqNo=self.sn, windowSize=self.N))
                self.ackStatus[self.sn]= 1
                # Send frame
                self.sock.sendall(str.encode(frame))
                # Start timer
                self.startTimer(self.sn)
                # Increase Sn
                self.increaseSn()

            
            # update sf if acknowledge received
            tmp = False
            while self.ackStatus[self.sf] == 2:
                self.ackStatus[self.sf] = 0
                self.increaseSf()
                self.data.dequeue()
                tmp = True

            if tmp:
                while self.ackStatus[self.sf] == 2:
                    self.ackStatus[self.sf] = 0
                    self.increaseSf()
                    self.data.dequeue()
            
            sleep()

    def recvAck(self):
        while True:
            # Listen for data
            data = self.sock.recv(1024)
            data = data.decode()
            # If ack is valid 
            decodedData = decodeData(data)
            if decodedData[0]:
                print("Received ack: ", decodedData[1])
                seqNo = int(decodedData[1][1:], 2)
                if decodedData[1][0] == '1':
                    # ACK
                    if self.sf <= seqNo <= self.sn:
                        if self.timers[seqNo]:
                            self.timers[seqNo].cancel()
                            self.timers[seqNo] = None
                        self.ackStatus[seqNo] = 2
                        # self.data.dequeue()
                        # self.increaseSf()
                else:
                    # NAK
                    if self.sf <= seqNo <= self.sn:
                        self.resendFramesSeqNo.add(seqNo)

            print("sf: ", self.sf, "sn: ", self.sn)
            # Unblock event
            self.sendThreadEvent.set()


    def increaseSn(self):
        self.sn += 1
        if self.sn > self.snmax:
            self.sn = 0

    def increaseSf(self):
        self.sf += 1
        if self.sf > self.snmax:
            self.sf = 0

    def startTimer(self, seqNo):
        if self.timers[seqNo]:
            self.timers[seqNo].cancel()
        self.timers[seqNo] = threading.Timer(self.timeout, self.resendFrame, [seqNo])
        self.timers[seqNo].start()

    def resendFrame(self, seqNo):
        self.resendFramesSeqNo.add(seqNo)
        self.sendThreadEvent.set()


    def pushData(self, data):
        frames = buildFrames(data, frameSize=8)
        for i in frames:
            self.data.enqueue(i)



sender = Sender('127.0.0.1', 8081, 4)

sender.pushData("0110100001100101011011010110110001101111011101010111100101110011")
# 01101000   01100101  01101101 01101100 01101111 01100111 01110101 01111001 01110011 
                 
sender.startProcess()