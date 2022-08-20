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

rec_ip =  input('Enter receiver ip: ')
rec_port = input('Enter receiver port: ')

ClientMultiSocket.sendall(str.encode(f'connect:{rec_ip}:{rec_port}'))
sleep(1)
ClientMultiSocket.sendall(str.encode('Hello'))

while True:
    data = ClientMultiSocket.recv(1024)
    if not data:
        break
    data = data.decode('utf-8')
    print(data)
    if data == 'disconnect':
        break
ClientMultiSocket.close()