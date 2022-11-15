import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 6777))

msg = input("enter message : ")
s.send(msg.encode())
msg = s.recv(1024)
print(msg.decode())