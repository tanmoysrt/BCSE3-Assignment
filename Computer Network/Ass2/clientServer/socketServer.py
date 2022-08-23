import socket
from _thread import *

# Purpose
# 1. Manage connection pool
# 2. Manage client requests
# 3. Bi-directional communication
# -> Sender can send data to Receiver
# -> Receiver can send data to Sender

class SocketServer:

    clientConnectionInstances = {}
    senderReceiver = {}
    # map of senderIp:senderPort to receiverIp:receiverPort
    # map of receiverIp:receiverPort to senderIp:senderPort

    def __init__(self, host, port):
        self.host = host
        self.port = port
        self.socket = socket.socket()
    
    def __repr__(self) -> str:
        return f"SocketServer({self.host}, {self.port})"
    
    def start(self):
        try:
            self.socket.bind((self.host, self.port))
        except socket.error as e:
            print(str(e))
        print('Server is online ...')
        self.socket.listen(5)
    
    def startAcceptConnections(self):
        while True:
            client, address = self.socket.accept()
            # Store client connection instance in dictionary
            SocketServer.clientConnectionInstances[f"{address[0]}:{address[1]}"] = client
            print('Connected to: ' + address[0] + ':' + str(address[1]))
            # Start a new thread to handle client requests
            start_new_thread(SocketServer.handleClient, (address[0], str(address[1])))

    @staticmethod
    def handleClient(clientIp, clientPort):
        address = f"{clientIp}:{clientPort}"
        # Get client connection instance from dictionary
        client = SocketServer.clientConnectionInstances[address]

        # Start listening for client requests
        while True:
            try:
                data = client.recv(1024)
                # Check whether client sent data
                if not data:
                    break
                data = data.decode('utf-8')
                # Check for special command to connect to receiver
                if str(data).startswith("connect:"):
                    splittedData = data.split(":")
                    if(len(splittedData) == 3):
                        receiverAddress = f"{splittedData[1]}:{splittedData[2]}"
                        # Check whether receiver address found
                        if receiverAddress in SocketServer.clientConnectionInstances:
                            # Check if receiver address is found in senderReceiver map
                            if receiverAddress in SocketServer.senderReceiver or address in SocketServer.senderReceiver:
                                print("Already connected to receiver")
                            else:
                                # Store in sender receiver map
                                SocketServer.senderReceiver[address] = receiverAddress
                                SocketServer.senderReceiver[receiverAddress] = address
                                print(f"Setup done ! Sender {address} is connected to receiver {receiverAddress}")
                        else:
                            print(f"Receiver {receiverAddress} not found")
                    else:
                        continue
                # Else consider as normal data
                else:
                    # Get receiver address from senderReceiver map
                    receiverAddress = None
                    if address in SocketServer.senderReceiver:
                        receiverAddress = SocketServer.senderReceiver[address]
                    else:
                        print(f"No receiver found for sender {address}")
                        continue
                    # Check whether receiver instance found
                    receiverInstance = SocketServer.clientConnectionInstances[receiverAddress]
                    if receiverInstance is None:
                        print(f"Receiver {receiverAddress} not found")
                        continue
                    # Send data to receiver    
                    receiverInstance.sendall(str.encode(SocketServer.modifyData(data)))
                    print(f"{address} ---> {receiverAddress} --> {data}")

            except (BrokenPipeError, ConnectionResetError, OSError):
                    print(f"Receiver {receiverAddress} is offline")
                    break
            except Exception as e:
                print(str(e))
                break
                
        client.close()

        if address in SocketServer.clientConnectionInstances:
            del SocketServer.clientConnectionInstances[address]

        if address in SocketServer.senderReceiver:
            # Get receiver address from senderReceiver map
            receiverAddress = SocketServer.senderReceiver[address]
            # Get receiver instance from clientConnectionInstances map
            if receiverAddress in  SocketServer.clientConnectionInstances:
                receiverInstance = SocketServer.clientConnectionInstances[receiverAddress]
                receiverInstance.send(str.encode(f"disconnect:"))
                # Close receiver instance
                receiverInstance.close()
                del SocketServer.clientConnectionInstances[receiverAddress]
                del SocketServer.senderReceiver[receiverAddress]
            # Delete own address from senderReceiver map
            del SocketServer.senderReceiver[address]

        print(f"Client {address} has disconnected")



    @staticmethod
    def modifyData(data):
        # An method to be overridden by child class
        return data

    def closeAllConnections(self):
        self.socket.close()
        print("Server is offline")

# tmp = SocketServer(host='127.0.0.1', port=8081)
# tmp.start()
# tmp.startAcceptConnections()