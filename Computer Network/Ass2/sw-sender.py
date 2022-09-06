import socket
import threading
from time import sleep

from helpers import buildFrames, decodeData, encodeData


# FRAME FORMAT:
# For a frame with length of 8, the format is:
# [sn][data][parity]
# [2 ][  8 ][  4   ]

# Ack frame format:
# [sn][parity]
# [ 4 ][  4   ]

class Sender:
    def __init__(self, host, port, timeout):
        self.host = host
        self.port = port
        self.timeout = timeout
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))
        self.ackReceivedEvent = threading.Event()
        self.ackReceivedEvent.clear()

        self.data = []
        self.frameIndex = 0
        self.sn = 0
        self.snmax = 1

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
            if self.frameIndex >= len(self.data):
                print("No more frames to send")
                break
            # Prepare fram e-- by taking the sn and the element of window
            frame = encodeData(str(bin(self.sn)[2:]).zfill(2)+self.data[self.frameIndex])
            print("[SEND] new frame ", frame, " with sn ", self.sn)
            # Increase seq no
            self.increaseSn()
            # Icrease frame index
            self.frameIndex += 1
            # Send a frame 
            self.sock.sendall(str.encode(frame))
            # Wait for ack with a timeout
            self.ackReceivedEvent.clear()
            isNotified = self.ackReceivedEvent.wait(timeout=self.timeout)
            if not isNotified:
                print("[RESEND] resending frame ", frame, " with sn ", self.sn)
                self.frameIndex -= 1
                self.decreaseSn()

            # If timeout, resend the frame -- run the loop again
            

    def recvAck(self):
        while True:
            # Listen for data
            data = self.sock.recv(1024)
            data = data.decode()
            # If ack is valid 
            decodedData = decodeData(data)
            if decodedData[0]:
                seqNo = int(decodedData[1], 2)
                # If ack sn == current sn, just emit ackreceived event
                if seqNo != self.sn:
                    # Else ack sn != current sn, set sn to ack sn , emit ackreceived event
                    self.sn = seqNo
                    # Set frame index to previous frame index
                    self.frameIndex = max(self.frameIndex-1, 0)
                    print("[ACK] ack received with sn ", seqNo)
                else:
                    print("[ACK] ack discarded as rn not macthed")
                # Emit ackreceived event
                self.ackReceivedEvent.set()
            else:
                print("[DISCARD] discarding ACK due to error")


    def increaseSn(self):
        self.sn += 1
        if self.sn > self.snmax:
            self.sn = 0

    def decreaseSn(self):
        self.sn -= 1
        self.sn = max(self.sn, 0)

    def pushData(self, data):
        frames = buildFrames(data, frameSize=8)
        for i in frames:
            self.data.append(i)



sender = Sender('127.0.0.1', 8081, 4)
sender.pushData("0110100001100101011011010110110001101111011101010111100101110011")
                 
sender.startProcess()