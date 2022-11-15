import socket
import time


domainiptable = {
    "google.com": "8.8.8.8",
    "tanmoy.codes" : "5.5.5.5"
}

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
s.bind(("127.0.0.1", 6777))


while True:
    msg, addr = s.recvfrom(1024)
    domainname  = msg.decode()
    if domainname in domainiptable:
        msg = domainiptable[domainname]
    else:
        msg = "not_found"
    time.sleep(0.1)
    s.sendto(msg.encode(), addr)