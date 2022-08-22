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
                    # TODO acknowledge data
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
            # TODO write code

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