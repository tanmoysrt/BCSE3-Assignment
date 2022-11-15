import socket

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)

msg = input("enter domain name : ")
s.sendto(msg.encode(), ("127.0.0.1", 6777))
msg = s.recv(1024)
print(msg.decode())
s.close()