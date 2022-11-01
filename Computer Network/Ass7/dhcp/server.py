from re import S
import socket
import random
import string
from threading import Thread
from time import sleep
from turtle import pen
from frame import Frame

class DhcpServer:
    def __init__(self, mac, starting_ip="192.168.0.2", subnet_mask="255.255.255.0", dhcp_client_port=1168, dhcp_server_port=1167):
        self.mac = mac
        self.dhcp_client_port = dhcp_client_port
        self.dhcp_server_port = dhcp_server_port

        self.dns_ip = "8.8.8.8"
        self.subnet_mask = subnet_mask
        self.gateway_ip = "192.168.0.1"

        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP) 
        # Enable port reusage
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        # Enable broadcasting mode
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

        # Bind to Client Port
        self.sock.bind(('', self.dhcp_server_port))

        self.starting_ip = [int(x) for x in starting_ip.split(".")]
        self.subnet_mask = [int(x) for x in subnet_mask.split(".")]

        # Transaction ID for DHCP
        self.transaction_id_offerred_ip = {}
        self.available_ips = set()

        # Lease time table
        self.lease_time_table = {}


    def broadcast(self, message):
        self.sock.sendto(message.encode(), ("<broadcast>", self.dhcp_client_port))
    
    def receiveMsg(self):
        data, addr = self.sock.recvfrom(1024)
        return data.decode()

    def startProcess(self):
        expiry_thread = Thread(target=self.timeoutExpiry)
        expiry_thread.start()
        while True:
            request_frame = Frame.decode(self.receiveMsg())
            if request_frame.requestType == "1":
                if request_frame.messageType == "1":
                    # Wait for DHCP Discover request
                    print("DHCP Discover request received from ", request_frame.clientMAC)
                    # Generate IP
                    ip = self.generateNextIP()
                    self.transaction_id_offerred_ip[request_frame.transactionID] = ip

                    # DHCP OFFER
                    offer = Frame()
                    offer.requestType = "2"
                    offer.messageType = "2"
                    offer.clientMAC = request_frame.clientMAC
                    offer.clientIP = ip
                    offer.transactionID = request_frame.transactionID
                    offer.hostName = request_frame.hostName
                    # sleep(2)
                    print("Sending DHCP Offer to ", request_frame.clientMAC)
                    self.broadcast(offer.encode())

                    # Wait for DHCP Request
                    request = Frame.decode(self.receiveMsg())
                    if request.requestType == "1" and request.messageType == "3":
                        print("DHCP Request received from ", request.clientMAC)
                        # sleep(2)
                        # Check if transaction id is same
                        if request.transactionID in self.transaction_id_offerred_ip:
                            # Check if IP is same
                            if self.transaction_id_offerred_ip[request.transactionID] == request.clientIP and (request.clientIP in self.available_ips or request.clientIP not in self.lease_time_table):
                                # DHCP ACK
                                ack = Frame()
                                ack.requestType = "2"
                                ack.messageType = "5"
                                ack.clientMAC = request.clientMAC
                                ack.clientIP = request.clientIP
                                ack.hostName = request.hostName
                                ack.transactionID = request.transactionID
                                ack.dnsIP = self.dns_ip
                                ack.subnetMask = '.'.join([str(x) for x in self.subnet_mask])
                                ack.gatewayIP = self.gateway_ip
                                ack.leaseTime = "10"
                                self.broadcast(ack.encode())
                                # Delete from available ips
                                if request.clientIP in self.available_ips:
                                    self.available_ips.remove(request.clientIP)
                                self.lease_time_table[request.clientIP] = 10
                                print("DHCP ACK sent to ", request.clientMAC)
                            else:
                                print("IP address not same as offerred")
                        else:
                            print("Transaction ID not same as offerred")
                elif request_frame.messageType == "3":
                    # DHCP ACK
                    ack = Frame()
                    ack.requestType = "2"
                    ack.messageType = "5"
                    ack.clientMAC = request.clientMAC
                    ack.clientIP = request.clientIP
                    ack.hostName = request.hostName
                    ack.transactionID = request.transactionID
                    ack.dnsIP = self.dns_ip
                    ack.subnetMask = '.'.join([str(x) for x in self.subnet_mask])
                    ack.gatewayIP = self.gateway_ip
                    ack.leaseTime = "10"
                    self.broadcast(ack.encode())
                    # Delete from available ips
                    if request.clientIP in self.available_ips:
                        self.available_ips.remove(request.clientIP)
                    self.lease_time_table[request.clientIP] = 10

    def timeoutExpiry(self):
        while True:
            ips_need_to_be_removed = []
            for ip, time in self.lease_time_table.items():
                if time <= 0:
                    ips_need_to_be_removed.append(ip)
                    print("IP address released ", ip)
                else:
                    self.lease_time_table[ip] -= 1

            for ip in ips_need_to_be_removed:
                del self.lease_time_table[ip]
                self.available_ips.add(ip)
            sleep(1)

    def generateIP(self):
        # TODO
        return ".".join([str(x) for x in self.starting_ip])

    def generateNextIP(self):
        # TODO do according to subnet mask
        self.starting_ip[3] += 1
        return ".".join([str(x) for x in self.starting_ip])

    def close(self):
        self.sock.close()


if __name__ == "__main__":
    server = DhcpServer("02:02:01:02:02:00")
    server.startProcess()
    server.close()