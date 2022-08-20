from socketServer import SocketServer
import random

class Channel(SocketServer):
    def __init__(self, host, port):
        super().__init__(host, port)
        # Update modifyData static method of  SocketServer with  Channel's
        SocketServer.modifyData = Channel.modifyData

    @staticmethod
    def modifyData(data):
        print(data)
        return data
    
    @staticmethod
    def injectErrorInData(data, loopC=5):
        for _ in range(loopC):
            random_bit_location = random.randint(0, len(data)-1)
            data = data[:random_bit_location] +  ['0','1'][random.randint(0,1)] + data[random_bit_location+1:]
        return data



if __name__ == "__main__":
    print("Socket Server[Channel] is starting....")
    server = Channel(host='127.0.0.1', port=8081)
    server.start()
    server.startAcceptConnections()
