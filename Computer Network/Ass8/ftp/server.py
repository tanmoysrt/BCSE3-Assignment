import socket
import subprocess
from threading import Thread
import os
from time import sleep

def runInTerminal(command, cwd):
    data  = subprocess.run(command, shell=True, capture_output=True, cwd=cwd)
    if data.returncode == 0:
        return (True, data.stdout.decode('utf-8'))
    else:
        return (False, data.stderr.decode('utf-8'))

class FTPServer:
    def __init__(self, host='127.0.0.1', dataPort=3001, commandPort=3002) -> None:
        self.host = host
        self.dataPort = dataPort
        self.commandPort = commandPort
        self.currentDirectoryUser = {}
        # Create the UDP socket
        self.commandSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        # Create the TCP socket
        self.dataSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        # Bind the socket to the port
        self.commandSocket.bind((host, commandPort))
        self.dataSocket.bind((host, dataPort))
        # Listen for incoming connections on data socket
        self.dataSocket.listen(1)
        # start process
        self.startProcess()

    def startProcess(self):
        commandServer = Thread(target=self.commandServer)
        dataServer = Thread(target=self.dataServer)
        commandServer.start()
        dataServer.start()
        commandServer.join()
        dataServer.join()
        

    def commandServer(self):
        while True:
            data, address = self.commandSocket.recvfrom(1024)
            dataSplitted = data.decode('utf-8').split(' ')
            transactionToken = dataSplitted[0]
            command = dataSplitted[1].strip()
            commandArgs = dataSplitted[2:]

            # Store the current directory of the user
            if transactionToken not in self.currentDirectoryUser:
                self.currentDirectoryUser[transactionToken] = os.getcwd()

            dataToSend = "Command not found \n"

            if command == 'ls':
                status, dataToSend = runInTerminal('ls -l', self.currentDirectoryUser[transactionToken])
            elif command == 'pwd':
                dataToSend = self.currentDirectoryUser[transactionToken] + "\n"
            elif command == 'cd':
                status, dataToSend = runInTerminal('cd ' + commandArgs[0], self.currentDirectoryUser[transactionToken])
                # Update current directory record
                if status:
                    self.currentDirectoryUser[transactionToken] = runInTerminal('cd ' + commandArgs[0] + "&& pwd", self.currentDirectoryUser[transactionToken])[1].strip()
            elif command == 'cat':
                status, dataToSend = runInTerminal('cat ' + commandArgs[0], self.currentDirectoryUser[transactionToken])
            elif command == 'del':
                status, dataToSend = runInTerminal('rm ' + commandArgs[0], self.currentDirectoryUser[transactionToken])
            
            self.commandSocket.sendto(dataToSend.encode('utf-8'), address)
                            

    def dataServer(self):
        while True:
            connection, address = self.dataSocket.accept()
            dataServer = Thread(target=self.handleDataConnection, args=(connection, address))
            dataServer.start()
            dataServer.join()

    def handleDataConnection(self, connection, address):
        while True:
            data = connection.recv(1024)
            if data:
                dataSplitted = data.decode('utf-8').split(' ')
                transactionToken = dataSplitted[0]
                command = dataSplitted[1].strip()
                commandArgs = dataSplitted[2:]

                if command == 'get':
                    filename = os.path.join(self.currentDirectoryUser[transactionToken],  commandArgs[0])
                    #  Send whether found the file
                    if os.path.isfile(filename):
                        connection.send("found".encode('utf-8'))
                    else:
                        connection.send("not_found".encode('utf-8'))
                        continue
                    sleep(1)
                    with open(filename, 'rb') as file:
                        data = file.read(1024)
                        while data:
                            connection.send(data)
                            data = file.read(1024)
                        file.close()
                    sleep(1)
                    connection.send(b'EOF')
                elif command == 'put':
                    # Accept incoming file and save
                    filename = os.path.join(self.currentDirectoryUser[transactionToken],  commandArgs[0])
                    with open(filename, 'wb') as file:
                        data = connection.recv(1024)
                        while data:
                            if data == b'EOF':
                                break
                            file.write(data)
                            data = connection.recv(1024)
                        file.close()
            else:
                break

if __name__ == '__main__':
    server = FTPServer()
    server.startProcess()