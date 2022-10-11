import socket
import time
import random
import sys

class Station:
    def __init__(self, index, host, port):
        self.index = index
        self.host = host
        self.port = port

        self.socketNew = socket.socket()
        self.socketNew.connect((host, port))

    def startSending(self):
        data = input("Enter data to send: ")
        self.socketNew.send(data.encode())

    def startReceiving(self):
        while True:
            data = self.socketNew.recv(1024).decode()
            if not data:
                break
            print("Received from channel: " + data)
            data = int(data)
            if data == -1:
                val = 0
            elif data == 1:
                val = 1
            else:
                val = "silent"
            print("Value of received bit is " + str(val))
        self.socketNew.close()

    def startProcess(self):
        self.startSending()
        self.startReceiving()

if __name__ == '__main__':
    station = Station(sys.argv[1], "127.0.0.1", 8082)
    station.startProcess()
    