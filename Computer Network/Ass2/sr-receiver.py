from audioop import tomono
from cmath import sqrt
from queue import Queue
import socket
import threading
from time import sleep

from helpers import decodeData, generateACK

# FRAME FORMAT:
# For a frame with length of 8, the format is:
# [sn][data][parity]
# [2 ][  8 ][  4   ]

# Ack frame format:
# [type][sn][parity]
# [  1 ][ 4 ][  4   ]
# Type -> ACK -> 1, NAK -> 0


class Receiver:
    def __init__(self, host, port, N=4):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))
        self.N = N
        self.rn = 0
        # tmp = int(sqrt(N).real)
        self.rnmax = 2**(N-1)
        
        self.data = []
        self.buffer = ["0"]*self.N

        self.nakSent = False
        self.ackNeeded = False

        self.marked = [False]*self.N

    def startProcess(self):
        # self.sendACKThread = threading.Thread(target=self.sendACK)
        self.receiverThread = threading.Thread(target=self.recv)
        self.receiverThread.start()
        # self.sendACKThread.start()


    def recv(self):
        while True:
            # Receieve frame
            data = self.sock.recv(1024)
            if not data:
                continue
            if data == "disconnect:" : break
            data = data.decode()
            print("data", data)

            # Decode frame
            decodedData = decodeData(data)
            # Check if valid
            if decodedData[0]:
                print("[ACCEPTED] Valid frame")
                # Extract frame and seq no
                data = decodedData[1]
                seqNo = int(data[:2], 2)
                frame = data[2:]
                # if seqno != rn
                if seqNo != self.rn and not self.nakSent:
                    self.sendNAK()
                    self.nakSent = True
                
                # If seq no == rn, save data and send ack for next frame
                if self.rn <= seqNo <= self.rnmax and not self.marked[seqNo]:
                    # Store frame
                    self.buffer[seqNo] = frame
                    # Mark Received
                    self.marked[seqNo] = True
                    while self.marked[self.rn]:
                        self.data.append(self.buffer[self.rn])
                        self.marked[self.rn] = False
                        self.rn = (self.rn+1)%self.N
                        self.ackNeeded = True
                    
                    if self.ackNeeded:
                        self.sendACK()
                        self.ackNeeded = False
                        self.nakSent = False
            else:
                print("[DISCARD] Discarding frame due to error")
                        
            self.printData()

    def sendACK(self):
        print("[ACK] Sending ACK for ", self.rn)
        data = str.encode(generateACK(self.rn, with_parity=True, for_selective_repeat=True, isNak=False))
        self.sock.sendall(data)
        sleep(0.1)

    def sendNAK(self):
        print("[NAK] Sending NAK for ", self.rn)
        data = str.encode(generateACK(self.rn, with_parity=True, for_selective_repeat=True, isNak=True))
        self.sock.sendall(data)
        sleep(0.1)

    def printData(self):
        print("Data ", end="")
        for frame in self.data:
            print(frame, end="", flush=True)
        print()

receiver = Receiver("127.0.0.1", 8081)
receiver.startProcess()