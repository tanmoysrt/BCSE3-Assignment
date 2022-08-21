from time import sleep

from clientServer.client import Client


class Sender(Client):
    def __init__(self, host, port):
        super().__init__(host, port)

    def setupBeforeProcess(self):
        receiver_ip = input("Enter receiver ip: ")
        receiver_port = input("Enter receiver port: ")
        self.sock.sendall(str.encode(f'connect:{receiver_ip}:{receiver_port}'))
        sleep(2)

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
                self.closeConnection()
    
if __name__ == '__main__':
    sender = Sender('127.0.0.1', 8081)
    try:
        sender.connectToServer()
        sender.startProcess()
    except KeyboardInterrupt:
        sender.closeConnection()
        exit()
    except Exception as e:
        print(str(e))
        sender.closeConnection()
        exit()