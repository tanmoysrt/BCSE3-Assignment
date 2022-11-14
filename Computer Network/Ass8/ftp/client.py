import socket
from threading import Thread
import random


class FTPClient:
    def __init__(self, host='127.0.0.1', dataPort=3001, commandPort=3002) -> None:
        self.host = host
        self.dataPort = dataPort
        self.commandPort = commandPort
        # User token -- to track the user operations and manage session
        self.userToken = str(random.randint(111111111, 999999999))
        # Buffer for tcp commands
        self.commandBufferForTCP = []
        # Create the UDP socket
        self.commandSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # Create the TCP socket
        self.dataSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # Bind the socket to the port
        self.dataSocket.connect((host, dataPort))

    def startProcess(self):
        commandClient = Thread(target=self.commandClient)
        dataClient = Thread(target=self.dataClient)
        commandClient.start()
        dataClient.start()
        commandClient.join()
        dataClient.join()

    def commandClient(self):
        while True:
            command = input('Enter command: ')
            if command == 'exit':
                break
            elif command.startswith("get") or command.startswith("put"):
                self.commandBufferForTCP.append(command)
                print(f"Command `{command}` will be relayed soon.")
            else:
                self.commandSocket.sendto((self.userToken + " " + command).encode('utf-8'), (self.host, self.commandPort))
                data, address = self.commandSocket.recvfrom(2048)
                print(data.decode('utf-8'), end="")

    def dataClient(self):
        while True:
            if len(self.commandBufferForTCP) > 0:
                command = self.commandBufferForTCP.pop(0)
                print(f"\rSending command `{command}`")
                self.dataSocket.send((self.userToken + " "+command).encode('utf-8'))
                # For download file
                '''
                get <filename>

                listen for data
                on data == "EOF" break
                '''
                if command.startswith("get"):
                    data = self.dataSocket.recv(1024).decode('utf-8')
                    if data == "found":
                        filename = command.split(" ")[1]
                        print(f"\rDownloading file {filename}", end="")
                        file = open(filename, "wb")
                        while True:
                            data = self.dataSocket.recv(1024)
                            if data == b"EOF":
                                break
                            file.write(data)
                        file.close()
                        print(f"\r `{filename}` downloaded successfully.")
                    elif data == "not_found":
                        print(f"\rFile not found.")
                elif command.startswith("put"):
                    # For upload file
                    '''
                    put <local_file_directory>
                    will be uploaded in that folder user currently in ftp server

                    end with "EOF"
                    '''
                    filename = command.split(" ")[1]
                    print(f"\rUploading file {filename}", end="")
                    file = open(filename, "rb")
                    while True:
                        data = file.read(1024)
                        if not data:
                            break
                        self.dataSocket.send(data)
                    self.dataSocket.send(b"EOF")
                    file.close()
                    print(f"\r `{filename}` uploaded successfully.")
            else:
                continue

if __name__ == '__main__':
    client = FTPClient()
    client.startProcess()