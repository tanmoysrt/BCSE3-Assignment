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
        self.ackThreadNotification = threading.Event()
        self.ackThreadNotification.clear()
        self.N = N

        # Its to send nak and ack from sendAck thread
        self.ackSeqNos = Queue()
        # ACK -> (1, data)
        # NAK -> (0, data)

        self.data = []
        self.rn = 0
        tmp = int(sqrt(N).real)
        self.rnmax = 2**tmp-1

        self.receivedStatus = [False]*self.N
        self.dataWindow = ["0"]*self.N

    def startProcess(self):
        self.sendACKThread = threading.Thread(target=self.sendACK)
        self.receiverThread = threading.Thread(target=self.recv)
        self.receiverThread.start()
        self.sendACKThread.start()


    def recv(self):
        while True:
            # Receieve data
            data = self.sock.recv(1024)
            if data == "disconnect:" : break
            data = data.decode()
            # Decode data
            decodedData = decodeData(data)
            # Check if valid
            if decodedData[0]:
                # Extract frame and seq no
                data = decodedData[1]
                seqNo = int(data[:2], 2)
                frame = data[2:]
                # If seq no == rn, save data and send ack for next frame
                if 0 <= seqNo <= self.rnmax and self.receivedStatus[seqNo] == False:
                    self.receivedStatus[seqNo] = True
                    self.dataWindow[seqNo] = frame
                    self.ackSeqNos.put((1, seqNo))
                # Rebuild frame and slide window if required

                #  Check from current rn to rnmax
                for i in range(0, self.rnmax+1):
                    if i < self.rn:
                        continue
                    if self.receivedStatus[i]:
                        self.data.append(self.dataWindow[i])
                        self.receivedStatus[i] = False
                        self.dataWindow[i] = "0"
                        self.increaseRn()
                    else:
                        break
                
                # Check from 0 to rn
                if self.rn == 0:
                    for i in range(0, self.rnmax+1):
                        if self.receivedStatus[i]:
                            self.data.append(self.dataWindow[i])
                            self.receivedStatus[i] = False
                            self.dataWindow[i] = "0"
                            self.increaseRn()
                        else:
                            break

    def sendACK(self):
        while True:
            while self.ackSeqNos.empty():
                sleep(0.1)

            ackData = self.ackSeqNos.get()
            data = str.encode(generateACK(ackData[1], with_parity=True, for_selective_repeat=True, isNak=(ackData[0]==0)))
            if ackData[0] == 0:
                print("NAK sent | Seq No ", ackData[1])
            else:
                print("ACK sent | Seq No ", ackData[1])
            self.sock.sendall(data)
            sleep(0.02)
            self.printData()

    def increaseRn(self):
        self.rn += 1
        if self.rn > self.rnmax:
            self.rn = 0

    def printData(self):
        print("Data : ", end="", flush=True)
        for i in self.data:
            print(i, end="", flush=True)
        print()

receiver = Receiver("127.0.0.1", 8081)
receiver.startProcess()