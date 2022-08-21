from random import randint, random
import threading
from time import sleep, time
from traceback import print_tb
from clientServer.client import Client
from errorDetectionModules.helper import ReadNoOfZerosAndOnes


class Receiver(Client):

    def __init__(self, host, port):
        super().__init__(host, port)
        self.data = ""
        self.sendAcknowledgement = False

    def setupBeforeProcess(self):
        pass

    def sendData(self):
        while True:
            if self.killed:
                break
            try:
                self.condition.acquire()
                if self.sendAcknowledgement:
                    self.sock.sendall(str.encode("ack:"))
                    self.sendAcknowledgement = False
                # Wait for receiveData thread to receive data
                self.condition.wait()
                self.condition.release()
            except (BrokenPipeError, ConnectionResetError):
                self.closeConnection()
                break
            

    def receiveData(self):
        while True:
            if self.killed:
                break
            data = self.sock.recv(1024)
            if not data:
                break
            data = data.decode('utf-8')
            if data == 'disconnect:':
                self.closeConnection()
                break
            elif data == "end:":
                print("Received data : ", self.data)
                self.data = ""
            else:
                # Check data is OK or not
                if self.isValidData(data):
                    self.data = self.data + data[:-1]
                    self.sendAcknowledgement = True
                    self.condition.acquire()
                    self.condition.notifyAll()
                    self.condition.release()
                else:
                    self.sendAcknowledgement = False

    def isValidData(self, data):
        _ , noOfOnes = ReadNoOfZerosAndOnes(data)
        if noOfOnes % 2 == 0:
            return True
        return False

if __name__ == '__main__':
    receiver = Receiver('127.0.0.1', 8081)
    try:
        receiver.connectToServer()
        receiver.startProcess()
    except KeyboardInterrupt:
        receiver.closeConnection()
        exit()
    except Exception as e:
        print(str(e))
        receiver.closeConnection()
        exit()