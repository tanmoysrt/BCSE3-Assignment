from time import sleep, time
from clientServer.client import Client


class Receiver(Client):
    def __init__(self, host, port):
        super().__init__(host, port)

    def setupBeforeProcess(self):
        pass

    def sendData(self):
        while True:
            if self.killed:
                break
            try:
                self.sock.sendall(str.encode("HIII BRO"))
                sleep(0.1)
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
            print(data)
            if data == 'disconnect:':
                break
    

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