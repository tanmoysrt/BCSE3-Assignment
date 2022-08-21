from abc import abstractmethod
import socket
import threading
import time


class Client:
    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.killed = False
        self.condition = threading.Condition()

    def connectToServer(self):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            self.sock.connect((self.host, self.port))
            print('Connected to server')
        except socket.error as e:
            print(str(e))

    def startProcess(self):
        self.setupBeforeProcess()
        self.senderThread = threading.Thread(target=self.sendData)
        self.receiverThread = threading.Thread(target=self.receiveData)
        self.receiverThread.start()
        self.senderThread.start()

    @abstractmethod
    def sendData(self):
        raise NotImplementedError

    @abstractmethod
    def receiveData(self):
        raise NotImplementedError

    @abstractmethod
    def setupBeforeProcess(self):
        raise NotImplementedError

    def closeConnection(self):
        if self.killed : return
        self.killed = True
        print('Closing connection')
        self.condition.acquire()
        self.condition.notifyAll()
        self.condition.release()

        try:
            self.sock.close()
        except socket.error as e:
            print(str(e))
        print('Connection closed')


# client = Client('127.0.0.1',8081)
# client.connectToServer()
# client.closeConnection()