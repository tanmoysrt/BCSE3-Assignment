# Frame format
# Request Type [1-> request, 2-> reply] | Message Type | Transaction ID | Client MAC Address | Client IP Address | Lease Time | Subnet Mask | Gateway IP | DNS IP | Host Name



class Frame:
    def __init__(self):
        self.requestType = ""
        self.messageType = ""
        self.transactionID = ""
        self.clientMAC = ""
        self.clientIP = ""
        self.leaseTime = ""
        self.subnetMask = ""
        self.gatewayIP = ""
        self.dnsIP = ""
        self.hostName = ""
    
    @staticmethod
    def decode(frameString):
        frame = Frame()
        frameSplitted = frameString.split("|")
        frame.requestType = frameSplitted[0]
        frame.messageType = frameSplitted[1]
        frame.transactionID = frameSplitted[2]
        frame.clientMAC = frameSplitted[3]
        frame.clientIP = frameSplitted[4]
        frame.leaseTime = frameSplitted[5]
        frame.subnetMask = frameSplitted[6]
        frame.gatewayIP = frameSplitted[7]
        frame.dnsIP = frameSplitted[8]
        frame.hostName = frameSplitted[9]

        return frame

    def encode(self):
        return str(self.requestType) + "|" + str(self.messageType) + "|" + str(self.transactionID) + "|" + str(self.clientMAC) + "|" + str(self.clientIP) + "|" + str(self.leaseTime) + "|" + str(self.subnetMask) + "|" + str(self.gatewayIP) + "|" + str(self.dnsIP) + "|" + str(self.hostName)


if __name__ == "__main__":
    frame = Frame()
    s = frame.encode()
    print(s)
    x = s.split("|")
    print(x)
    print(len(x))