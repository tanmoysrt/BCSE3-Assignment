import socket
import time

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
s.bind(("127.0.0.1", 6777))


while True:
    msg, addr = s.recvfrom(1024)
    msg  = msg.decode()
    time.sleep(0.1)
    msg = ("[ECHO] "+msg).encode()
    s.sendto(msg, addr)