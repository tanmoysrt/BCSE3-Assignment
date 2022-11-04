import socket
from helpers import *
import  os

# Docs : https://datatracker.ietf.org/doc/html/rfc1035

UDP_IP_ADDRESS = "127.0.0.1"
UDP_PORT_NO = 53


serverSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverSock.bind((UDP_IP_ADDRESS, UDP_PORT_NO))

def resolveQuery(query:Query):
    record_path = os.path.join(os.path.dirname(__file__), "records", query.baseDomainName())
    # If the file exists, then we have a record for this domain
    if os.path.exists(record_path):
        answers = []
        nameservers = []
        # Read the file and parse the answers
        with open(record_path, "r") as f:
            for curLine in f:
                curLine = curLine.strip() # Strip to remove newlines
                curLineParts = curLine.split("\t")
                if len(curLineParts) < 3: continue
                if curLineParts[0] == query.domainName(endDot=True) and curLineParts[1] == query.queryClass():
                    if curLineParts[2] == query.queryType():
                        answers.append(curLine)
                    if curLineParts[2] == "NS":
                        nameservers.append(curLine)
        return (True, answers, nameservers)
    
    # else , we have no record for this domain
    return (False, [], [])


def process(data):
    transactionID = getTransactionID(data)
    flags = Flags.fromBytes(data, debug=False)
    QDCount = getQDCount(data) # Usually 1
    ANCount = getANCount(data)
    NSCount = getNSCount(data)
    # ARCount = getARCount(data)
    ARCount = 0
    query = Query.fromBytes(data)

    print("Transaction ID: " + transactionID)

    #  Find the record for this query
    result = resolveQuery(query)

    if result[0]:
        transactionID = data[:2]
        QDCount = (QDCount).to_bytes(2, "big")
        ANCount = len(result[1]).to_bytes(2, "big")
        NSCount = len(result[2]).to_bytes(2, "big")
        ARCount = (0).to_bytes(2, "big")
    else:
        transactionID = data[:2]
        QDCount = (QDCount).to_bytes(2, "big")
        ANCount = (0).to_bytes(2, "big")
        NSCount = (0).to_bytes(2, "big")
        ARCount = (0).to_bytes(2, "big")

    # Build the response
    # Header
    responseHeader = transactionID + flags.toBytes() + QDCount + ANCount + NSCount + ARCount
    print("Response Header: " + str(responseHeader))

    # Question
    responseQuestion = query.toBytes()
    print("Response Question: " + str(responseQuestion))

    # Body
    responseBody = b""
    for record in result[1]:
        record_split = record.split("\t")
        # Name | Class | Type | Val | TTL
        responseBody += resultRecordToBytes(record_split[2], record_split[1], record_split[3], int(record_split[4]))

    return responseHeader + responseQuestion + responseBody

while True:
    data, addr = serverSock.recvfrom(512)
    serverSock.sendto(process(data), addr)