import socket
from time import sleep

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)  # UDP
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
    sock.bind(('', 1168))
    sock.sendto("hello world".encode(), ("", 1167))
    sock.close()

main()