from time import sleep
import socket
import random
import string
from frame import Frame
from threading import Thread

class DhcpClient:
    def __init__(self, mac, host_name, dhcp_client_port=1168, dhcp_server_port=1167):
        self.mac = mac
        self.ip = '0.0.0.0'
        self.subnet_mask = '0.0.0.0'
        self.dns_ip = '0.0.0.0'
        self.gateway_ip = '0.0.0.0'
        self.lease_time = 0
        self.host_name = host_name
        self.dhcp_client_port = dhcp_client_port
        self.dhcp_server_port = dhcp_server_port

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP) 
        # Enable port reusage
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        # Enable broadcasting mode
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

        # Bind to Client Port
        self.sock.bind(('', self.dhcp_client_port))

        # Transaction ID
        self.transacionID = "-1"

    def broadcast(self, message):
        self.sock.sendto(message.encode(), ("<broadcast>", self.dhcp_server_port))
    
    def receiveMsg(self):
        data, addr = self.sock.recvfrom(2048)
        return data.decode()

    def startProcess(self):
        # Generate transaction id
        self.transacionID = ''.join(random.choices(string.ascii_lowercase + string.digits, k=8))
        receive_thread = Thread(target=self.receive_data)
        receive_thread.start()
        sleep(0.01)
        self.send_dhcp_discover()
        receive_thread.join()



    def send_dhcp_discover(self):
        # Send DHCP Discover
        frame = Frame()
        frame.requestType = 1
        frame.messageType = 1
        frame.transactionID = self.transacionID
        frame.clientMAC = self.mac
        frame.clientIP = '0.0.0.0'
        frame.hostName = self.host_name
        self.broadcast(frame.encode())

    def send_dhcp_request(self):
        frame = Frame()
        frame.requestType = 1
        frame.messageType = 3
        frame.transactionID = self.transacionID
        frame.clientMAC = self.mac
        frame.clientIP = self.ip
        frame.hostName = self.host_name
        self.broadcast(frame.encode())

    def receive_data(self):
        while True:
            receieved_frame_raw = self.receiveMsg()
            receieved_frame = Frame.decode(receieved_frame_raw)
            # DHCP OFFER
            if receieved_frame.requestType == "2" and receieved_frame.transactionID == self.transacionID and receieved_frame.messageType == "2" and receieved_frame.clientMAC == self.mac:
                self.ip = receieved_frame.clientIP
                # Send DHCP Request
                self.send_dhcp_request()

            # DHCP ACK
            if receieved_frame.requestType == "2" and receieved_frame.transactionID == self.transacionID  and receieved_frame.clientMAC == self.mac:
                if receieved_frame.messageType == "5":
                    self.ip = receieved_frame.clientIP
                    self.subnet_mask = receieved_frame.subnetMask
                    self.gateway_ip = receieved_frame.gatewayIP
                    self.dns_ip = receieved_frame.dnsIP
                    self.lease_time = int(receieved_frame.leaseTime)
                    self.printDetails()
                    lease_thread = Thread(target=self.handleLeaseTime)
                    lease_thread.start()
                elif receieved_frame.messageType == "6":
                    # NACK
                    print("DHCP NAK received from DHCP server. Listening again ...")

            
    def handleLeaseTime(self):
        sleep(self.lease_time+0.1)
        print("Lease time expired. Listening again ...")
        self.send_dhcp_request()

    def printDetails(self):
        print("-------------------------------------------------")
        print("Host        : ", self.host_name)
        print("IP          : ", self.ip)
        print("MAC         : ", self.mac)
        print("Subnet mask : ", self.subnet_mask)
        print("Gateway IP  : ", self.gateway_ip)
        print("DNS         : ", self.dns_ip)
        print("Lease time  : ", self.lease_time)
        print("-------------------------------------------------")


    def close(self):
        self.sock.close()


# Steps 
# 1. DHCP Discover
# 2. DHCP Request

if __name__ == "__main__":
    mac_id = "02:00:00:%02x:%02x:%02x" % (random.randint(0, 255),random.randint(0, 255), random.randint(0, 255))
    print("MAC ID: ", mac_id)
    client_name = "client-" + mac_id
    client = DhcpClient(mac_id, client_name)
    client.startProcess()
    client.close()