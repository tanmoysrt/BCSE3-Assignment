from cmath import sqrt
from queue import Queue
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

        self.data = Queue()
        self.sf = 0
        self.sn = 0


        tmp = int(sqrt(N).real)
        self.sw = 2**tmp-1

        self.oldFramesData = {}

        # Window frame status
        self.timers = [None]*(self.sw+1)

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
        self.resendFrameThread = threading.Thread(target=self.resendFrame)
        self.receiverACKThread.start()
        self.senderThread.start()
        self.resendFrameThread.start()

    def send(self):
        while True:
            if self.data.empty():
                print("end....")
                break

            if (self.sn - self.sf) < self.sw:
                # Prepare fram e-- by taking the sn and the element of window
                x = self.data.get()
                # Make frame
                frame =  encodeData(str(bin(self.sn)[2:]).zfill(2)+x)
                # Store frame
                self.oldFramesData[self.sn] = frame
                # Send frame
                self.sock.sendall(str.encode(frame))
                # Increment sn
                self.sn = (self.sn+1)%self.sw
                # Start timer
                self.startTimer(self.sn)

            sleep(0.1)

    def recvAck(self):
        while True:
            # Receive frae
            data = self.sock.recv(1024)
            data = data.decode()
            # If ack is valid 
            decodedData = decodeData(data)
            if decodedData[0]:
                # print("Received ack: ", decodedData[1])
                seqNo = int(decodedData[1][1:], 2)
                if decodedData[1][0] == '1':
                    # ACK
                    if self.sf < seqNo <= self.sn:
                        while self.sf < seqNo:
                            # Start timer
                            if self.timers[seqNo]:
                                self.timers[seqNo].cancel()
                                self.timers[seqNo] = None
                            self.sf = (self.sf+1)%(self.sw+1)
                else:
                    # NAK
                    if self.sf < seqNo <= self.sn:
                        self.resendFrameAndSetTimer(seqNo)

    def resendFrame(self):
        pass

    def startTimer(self, seqNo):
        if self.timers[seqNo]:
            self.timers[seqNo].cancel()
        self.timers[seqNo] = threading.Timer(self.timeout, self.resendFrame, [seqNo])
        self.timers[seqNo].start()

    def resendFrame(self, seqNo):
        frame =  self.oldFramesData[seqNo]
        self.sock.sendall(frame)
        sleep(0.1)

    def resendFrameAndSetTimer(self, seqNo):
        frame =  self.oldFramesData[seqNo] # It wil have encoded frame
        if len(frame) == 14:
            self.sock.sendall(frame)
            self.startTimer(seqNo)
            sleep(0.1)


    def pushData(self, data):
        frames = buildFrames(data, frameSize=8)
        for i in frames:
            self.data.put(i)




sender = Sender('127.0.0.1', 8081, 4)

sender.pushData("011010000110010101101101011011000110111101110101011110010111001101110011")
# 01101000   01100101  01101101 01101100 01101111 01100111 01110101 01111001 01110011 01110011 
# 01101000   01100101  01101101
sender.startProcess()