from time import sleep
import socket
import random
import string
from threading import Thread

# Frame format
# IPSender:MACSender:ipRequested:macRequested

class Client:
    def __init__(self, ip, mac):
        # Set ip and mac of self
        self.ip = ip
        self.mac = mac
        # Create socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP) 
        # Enable port reusage
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        # Enable broadcasting mode
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
        # Bind to Client Port
        self.sock.bind(('', 1148))
        # Arp Table
        self.arp_table = {}

        print("Enter `arp` to show arp table")
        print("Enter IP to send ARP request")


    def startProcess(self):
        send_thread = Thread(target=self.sendMsg)
        receive_thread = Thread(target=self.receiveMsg)
        send_thread.start()
        receive_thread.start()
        send_thread.join()
        receive_thread.join()
        self.sock.close()

    def sendMsg(self):
        while True:
            ip = input()
            if ip == "arp":
                self.printArpTable()
            elif ip in self.arp_table:
                print("Cached MAC : " + self.arp_table[ip])
            else:
                self.sock.sendto((f"{self.ip}:{self.mac}:{ip}:").encode(), ('<broadcast>', 1148))

    def receiveMsg(self):
        while True:
            data, addr = self.sock.recvfrom(1024)
            msg = data.decode()
            msg_splitted = msg.split(":")

            ipSender = msg_splitted[0]
            macSender = msg_splitted[1]

            ipRequested = msg_splitted[2]
            macRequested = msg_splitted[3]

            # Store the sender ip and mac in arp table if not already stored
            if ipSender != self.ip:
                self.arp_table[ipSender] = macSender

            if ipRequested == self.ip:
                # Check whether the mac address is already present in frame
                if macRequested == "":
                    self.sock.sendto(f"{ipSender}:{macSender}:{self.ip}:{self.mac}".encode(), ('<broadcast>', 1148))
            elif macRequested not in self.arp_table:
                if macRequested != "":
                    self.arp_table[ipRequested] = macRequested

    def printArpTable(self):
        print("\nIP\tMAC")
        for ip, mac in self.arp_table.items():
            print(ip + "\t" + mac)
        print()

    
if __name__ == "__main__":
    # Create client
    ip = input("Enter IP: ")
    mac = input("Enter MAC (enter -1 for random mac): ")
    if mac == "-1":
        mac = ''.join(random.choice(string.ascii_uppercase + string.digits) for _ in range(12))
    client = Client(ip, mac)
    client.startProcess()