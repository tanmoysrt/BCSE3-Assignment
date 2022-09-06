import socket
import threading

from helpers import decodeData, generateACK


class Receiver:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.host, self.port))
        self.frameReceived = threading.Event()
        self.frameReceived.clear()

        self.data = []
        self.rn = 0
        self.rnmax = 1


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
                if seqNo == self.rn:
                    self.data.append(frame)
                    self.increaseRn()
                    self.frameReceived.set()
                    self.printData()
                    print("[ACCEPT] Frame received ")
                # If seq no != rn, discard and send ack fagain
                else:
                    print("[DISCARD] seqNo not matched to rn")
                    self.frameReceived.set()
            else:
                print("[DISCARD] frame due to error")

    def sendACK(self):
        while True:
            # Wait for frame received event
            self.frameReceived.wait()
            print("[ACK] Sending ACK for frame ", self.rn)
            # Send ack for the frame
            self.sock.sendall(str.encode(generateACK(self.rn, with_parity=True)))
            # Clear frame received event
            self.frameReceived.clear()

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