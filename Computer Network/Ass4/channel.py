import json
import socket
import helpers


class Channel():

    def __init__(self, totalstations):
        self.totalstation = totalstations
        self.stationhost = '127.0.0.1'
        self.stationport = 8082
        self.stationconn = []
        self.walshtable = [[]]

    def initStations(self):
        stationSocket = socket.socket()
        stationSocket.bind((self.stationhost, self.stationport))
        stationSocket.listen(self.totalstation)
        for i in range(1, self.totalstation+1):
            conn = stationSocket.accept()
            self.stationconn.append(conn)
        print('Initiated all station connections')

    def closeStations(self):
        for conn in self.stationconn:
            conn[0].close()
        print('Closed all station connections')

    def generateWalshTable(self):
        print('Generating Walsh table ...')
        self.walshtable = helpers.generateWalshTable(self.totalstation)
    
    def process(self):
        data = []
        for i in range(self.totalstation):
            conn = self.stationconn[i]
            d = conn[0].recv(1024).decode()
            data.append(d)
            
        for i in range(self.totalstation):
            print('Station',i+1,'will send data:',end=' ')
            print(data[i])
        maxlen = 0
        for i in data:
            maxlen = max(maxlen,len(i))
        datavalue = []
        for d in data:
            tup = []
            for j in range(maxlen):
                if j < len(d):
                    if d[j] == '0':
                        tup.append(-1)
                    elif d[j] == '1':
                        tup.append(1)
                else:
                    tup.append(0)
            datavalue.append(tup)
            
        for i in range(maxlen):	
            print('---------------------------------------------')
            print('Sending bit',i+1,'of each station\'s data')
            finaldata = []			
            d = []
            c = []
            n = len(self.walshtable)
            for j in range(n):
                if j < self.totalstation:
                    d.append(datavalue[j][i])
                else:
                    d.append(1)
                c.append(self.walshtable[j])
                finaldata.append(0)
                
            for j in range(n):
                temp = helpers.multiplyScalar(c[j], d[j])
                finaldata = helpers.addTuples(finaldata,temp)
            print('Bit',i+1,'of each station is:', end=' ')
            print(d)	
            print('Final data is:'+json.dumps(finaldata))
            
            choice = input('Does any station want to receive data ? (y/n) ')
            while choice == 'y':
                # User input
                stationNo = int(input("Enter the station number: "))
                receiverNo = int(input("Enter the receiver station number: "))
                if stationNo > self.totalstation or stationNo <= 0 or receiverNo > self.totalstation or receiverNo <= 0:
                    # Invalid check
                    print('Invalid station number')
                else:
                    temp = helpers.multiplyTuples(finaldata, c[stationNo-1])
                    summ = sum(temp)
                    databit = str(summ//n)
                    conn = self.stationconn[receiverNo-1]
                    conn[0].sendto(databit.encode(), conn[1])
                    print('Multiplying final data with Code bits of sender station',stationNo)
                    print('The result is : ',temp)
                    print('the sum of result is:',summ)
                    print('THE DATA BIT OF STATION',stationNo,'is:',databit)
                    print('Data bit sent to receiver',receiverNo,'successfully!')

                    choice = input('Does any station want to receive data ? (y/n) ')
        

if __name__ == '__main__':
    totalstations = int(input('Enter number of stations: '))
    
    ch = Channel(totalstations)
    ch.generateWalshTable()
    helpers.displayWalshTable(ch.walshtable)
    ch.initStations()
    ch.process()
    ch.closeStations()
