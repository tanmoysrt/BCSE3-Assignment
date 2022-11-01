import socket
from time import sleep

# def main():
#     interfaces = socket.getaddrinfo(host=socket.gethostname(), port=None, family=socket.AF_INET)
#     allips = [ip[-1][0] for ip in interfaces]

#     print(allips)
#     return

#     msg = b'hello world'
#     while True:

#         for ip in allips:
#             print(f'sending on {ip}')
#             sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)  # UDP
#             sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
#             sock.bind((ip,0))
#             sock.sendto(msg, ("255.255.255.255", 5005))
#             sock.close()

#         sleep(2)


# main()


 
def main():
    bufferSize  = 1024

    
    sock = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
    sock.setsockopt
    sock.bind(('', 1167))
    print("UDP server up and listening")

    while(True):
        bytesAddressPair = sock.recvfrom(bufferSize)
        message = bytesAddressPair[0]
        address = bytesAddressPair[1]

        print(address, message)

    

        # Sending a reply to client

        sock.sendto("serrver broadcasted".encode(), address)

main()