import socket
from time import sleep
ClientMultiSocket = socket.socket()
host = '127.0.0.1'
port = 8081
print('Waiting for connection response')
try:
    ClientMultiSocket.connect((host, port))
except socket.error as e:
    print(str(e))

while True:
    data = ClientMultiSocket.recv(1024)
    if not data:
        break
    data = data.decode('utf-8')
    if data == 'disconnect:':
        break
    print(data)
    # break

# ClientMultiSocket.sendall(str.encode('Hello Bro'))
ClientMultiSocket.close()