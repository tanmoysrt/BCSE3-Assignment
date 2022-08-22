from datetime import datetime
import threading
from time import sleep, time

from clientServer.client import Client
from queue import Queue

from errorDetectionModules.helper import ReadNoOfZerosAndOnes, buildFrames

class Sender(Client):

    def __init__(self, host, port, requestTimeout):
        super().__init__(host, port)
        self.requestTimeout = requestTimeout
        self.acknowledgementReceived = False
        self.newData = False
        self.isResendFrame = False
        self.dataBuffer = Queue()
        self.timer = None
        self.readyToLoadData = True
        self.requestSendTime = 0

    def setupBeforeProcess(self):
        receiver_ip = input("Enter receiver ip: ")
        receiver_port = input("Enter receiver port: ")
        self.sock.sendall(str.encode(f'connect:{receiver_ip}:{receiver_port}'))
        sleep(1)

    def sendData(self):
        lastFrameSent = ""
        while True:
            if self.killed:
                break
            try:
                self.condition.acquire()
                if self.newData:
                    # If new data loads send it
                    lastFrameSent = self.dataBuffer.get()
                    self.sock.sendall(str.encode(lastFrameSent))
                    self.newData = False # Reset new data flag
                    # Start timer
                    self.startTimer()                


                elif self.isResendFrame:
                    # Resend old frame
                    self.sock.sendall(str.encode(lastFrameSent))
                    self.isResendFrame = False
                    self.acknowledgementReceived = False
                    self.startTimer()

                elif self.acknowledgementReceived:
                    self.acknowledgementReceived = False
                    # Send next frame
                    if self.dataBuffer.empty():
                        self.sock.sendall(str.encode("end:"))
                        self.cancelTimer()
                        sleep(0.05)
                        
                        self.readyToLoadData = True
                        self.isResendFrame = False
                        self.condition.notifyAll()
                    else:
                        lastFrameSent = self.dataBuffer.get()
                        self.sock.sendall(str.encode(lastFrameSent))
                        # Start timer
                        self.startTimer()
                
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
            elif data == "ack:":
                self.printRoundTripTime()
                self.condition.acquire()
                self.acknowledgementReceived = True
                self.cancelTimer()
                self.condition.notifyAll()
                self.condition.release()

    def setDataForSending(self, data):
        self.condition.acquire()
        while not self.readyToLoadData:
            self.condition.wait()

        frames = buildFrames(data, 8)
        for i in frames:
            _ , noOfOnes = ReadNoOfZerosAndOnes(i)
            partiyBit = "0" if (noOfOnes % 2) == 0 else "1"
            self.dataBuffer.put(i+partiyBit)
        
        self.newData = True
        self.readyToLoadData = False

        self.condition.notifyAll()
        self.condition.release()

    def resendFrame(self):
        print("Resending frame")
        self.condition.acquire()
        self.isResendFrame = True
        self.condition.notifyAll()
        self.condition.release()

    def cancelTimer(self):
        if self.timer is not None:
            self.timer.cancel()

    def startTimer(self):
        self.requestSendTime = datetime.now()
        self.cancelTimer()
        self.timer = threading.Timer(self.requestTimeout, self.resendFrame)
        self.timer.start()

    def printRoundTripTime(self):
        print(f"Round trip time: {(datetime.now() - self.requestSendTime).microseconds} microseconds")
    
if __name__ == '__main__':
    sender = Sender('127.0.0.1', 8081, 1)
    try:
        sender.connectToServer()
        sender.startProcess()
        sender.setDataForSending("1000000101111110")
        sender.setDataForSending("0111111010000001")
        sender.setDataForSending("0100011010111001")
         
        while True:
            sender.condition.acquire()
            while not sender.readyToLoadData:
                sender.condition.wait()
            sender.condition.release()
            data = input("Enter data: ")
            sender.setDataForSending(data)
    except KeyboardInterrupt:
        sender.closeConnection()
        exit()
    except Exception as e:
        print(str(e))
        sender.closeConnection()
        exit()