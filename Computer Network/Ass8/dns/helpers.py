### Required datas
queryTypesName = ["A", "NS", "MD", "MF", "CNAME", "SOA", "MB", "MG", "MR", "NULL", "WKS", "PTR", "HINFO", "MINFO", "MX", "TXT"]
queryClassesName = ["IN", "CS", "CH", "HS"]

### Functions
# Get Transaction ID from data
def getTransactionID(rawData:bytes):
    transactionIDRaw = rawData[:2]
    transactionID = ""
    for x in transactionIDRaw:
        transactionID += hex(x)[2:]
    return transactionID

# Get flags from data
class Flags:
    def __init__(self, QR, OPCODE, AA, TC, RD, RA, Z, RCODE):
        self.QR = QR
        self.OPCODE = OPCODE
        self.AA = AA
        self.TC = TC
        self.RD = RD
        self.RA = RA
        self.Z = Z
        self.RCODE = RCODE

    def __str__(self):
        return str(self.QR) + " " + str(self.OPCODE) + " " + str(self.AA) + " " + str(self.TC) + " " + str(self.RD) + " " + str(self.RA) + " " + str(self.Z) + " " + str(self.RCODE)
    
    @staticmethod
    def fromBytes(rawData:bytes, debug=False):
        #       first byte       |second byte
        # | 1 |  4   | 1 | 1 | 1 | 1 | 3|  4  |
        # | QR|Opcode| AA| TC| RD| RA| Z|RCODE|

        first_byte = ord(rawData[2:3])
        second_byte = ord(rawData[3:4])

        if debug:
            print("[DEBUG] First byte: " + str(first_byte)+" "+bin(first_byte))
            print("[DEBUG] Second byte: " + str(second_byte)+" "+bin(second_byte))

        QR = first_byte >> 7
        OPCODE = (first_byte  & 0b01111000 ) >> 3
        AA = (first_byte & 0b00000100) >> 2
        TC = (first_byte & 0b00000010) >> 1
        RD = (first_byte & 0b00000001)

        RA = second_byte >> 7
        Z = (second_byte & 0b1110000) >> 4
        RCODE = second_byte & 0b1111

        if debug:
            print("[DEBUG] QR: " + str(QR))
            print("[DEBUG] OPCODE: " + str(OPCODE))
            print("[DEBUG] AA: " + str(AA))
            print("[DEBUG] TC: " + str(TC))
            print("[DEBUG] RD: " + str(RD))
            print("[DEBUG] RA: " + str(RA))
            print("[DEBUG] Z: " + str(Z))
            print("[DEBUG] RCODE: " + str(RCODE))
        
        return Flags(QR, OPCODE, AA, TC, RD, RA, Z, RCODE)
    
    def toBytes(self):
        first_byte = (self.QR << 7) + (self.OPCODE << 3) + (self.AA << 2) + (self.TC << 1) + self.RD
        second_byte = (self.RA << 7) + (self.Z << 4) + self.RCODE
        return first_byte.to_bytes(1, "big") + second_byte.to_bytes(1, "big")

# Get QD count from data
def getQDCount(rawData:bytes):
    QDCountRaw = rawData[4:6]
    QDCount = ""
    for x in QDCountRaw:
        QDCount += hex(x)[2:]
    return int(QDCount)

# Get AN count from data
def getANCount(rawData:bytes):
    ANCountRaw = rawData[6:8]
    ANCount = ""
    for x in ANCountRaw:
        ANCount += hex(x)[2:]
    return int(ANCount)

# Get NS count from data
def getNSCount(rawData:bytes):
    NSCountRaw = rawData[8:10]
    NSCount = ""
    for x in NSCountRaw:
        NSCount += hex(x)[2:]
    return int(NSCount)

# Get AR count from data
def getARCount(rawData:bytes):
    ARCountRaw = rawData[10:12]
    ARCount = ""
    for x in ARCountRaw:
        ARCount += hex(x)[2:]
    return int(ARCount)

# Query Questions parser
class Query:
    def __init__(self, nameParts, type, qclass):
        self.nameParts = nameParts
        self.type = type
        self.qclass = qclass

    def baseDomainName(self):
        return self.nameParts[-2] + "." + self.nameParts[-1]

    def domainName(self, endDot=False):
        res = '.'.join(self.nameParts)
        if endDot and res[-1] != ".":
            res += "."
        return res

    def queryType(self):
        if self.type > 16:
            return "Unknown"
        return queryTypesName[self.type - 1]

    def queryClass(self):
        if self.qclass > 4:
            return "Unknown"
        return queryClassesName[self.qclass - 1]

    def __str__(self):
        return self.domainName() + " " + str(self.type) + " " + str(self.qclass)

    @staticmethod
    def fromBytes(rawData:bytes, debug=False):
        data = rawData[12:]
        if debug:
            print("[DEBUG] Data: ")
            print(data)
        # Format : {length of characters + [string]} untill 0x00

        domainParts = []

        totalLengthOfDomainPartsWithLengthCharacter = 0
        length = -1
        tmp = ""
        for byte in data:
            totalLengthOfDomainPartsWithLengthCharacter += 1
            if byte == 0: # End of parts
                if tmp != "":
                    domainParts.append(tmp)
                break

            if length == -1: # Yet not started, so first byte is length
                length = byte
            elif length == 0: # Read one part, so next byte is length
                domainParts.append(tmp)
                tmp = ""
                length = byte
            else: # Its part of domain name
                tmp += chr(byte)
                length -= 1
        
        tmp = data[totalLengthOfDomainPartsWithLengthCharacter:totalLengthOfDomainPartsWithLengthCharacter+4]
        # QType
        QTypeRaw = tmp[:2]
        QType = ""
        for x in QTypeRaw:
            QType += hex(x)[2:]
        QType = int(QType, 16)
        # QClass
        QClassRaw = tmp[2:]
        QClass = ""
        for x in QClassRaw:
            QClass += hex(x)[2:]
        QClass = int(QClass, 16)
        if debug:
            print("Domain Parts: " + str(domainParts))
            print("QType: " + str(QType))
            print("QClass: " + str(QClass))

        return Query(domainParts, QType, QClass)

    def toBytes(self):
        res = b""
        for part in self.nameParts:
            res += bytes([len(part)]) + bytes(part, "utf-8")
        res += b"\x00"
        res += self.type.to_bytes(2, "big")
        res += self.qclass.to_bytes(2, "big")
        return res

# Get type no from Query type name
def getTypeNoFromName(name):
    for i in range(len(queryTypesName)):
        if name == queryTypesName[i]:
            return i + 1
    return 0

# Get class no from Query class name
def getClassNoFromName(name):
    for i in range(len(queryClassesName)):
        if name == queryClassesName[i]:
            return i + 1
    return 0

# Result Record to bytes
def resultRecordToBytes(qTypeName, qClassName, val:str, ttl:int):
    print("Converting " + qTypeName + " " + qClassName + " " + val + " " + str(ttl))
    res = b"\xc0\x0c"
    res += getTypeNoFromName(qTypeName).to_bytes(2, "big")
    res += getClassNoFromName(qClassName).to_bytes(2, "big")
    res += ttl.to_bytes(4, "big")
    if qTypeName == "A":
        res += b"\x00\x04"
        res += bytes(map(int, val.split(".")))
    elif qTypeName == "AAAA":
        val = [int(x) for x in val.split(":")]
        res += len(val).to_bytes(2, "big")
        for x in val:
            res += x.to_bytes(2, "big")
    else:
        totalLen = 0
        data = b""
        for x in val.split("\n"):
            r = bytes(x, "utf-8")
            rLen = len(r)
            totalLen += rLen+1
            data += rLen.to_bytes(1, "big") + r

        res += totalLen.to_bytes(2, "big")
        res += data

    return res

    