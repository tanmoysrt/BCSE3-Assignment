from socketServer import SocketServer
import random

class Channel(SocketServer):
    def __init__(self, host, port):
        super().__init__(host, port)
        # Update modifyData static method of  SocketServer with  Channel's
        SocketServer.modifyData = Channel.modifyData

    @staticmethod
    def modifyData(data):
        if data in ["ack:", "end:", "disconnect:"]:
            return data
        return Channel.injectErrorInData(data, loopC=1)
    
    @staticmethod
    def injectErrorInData(data, loopC=5):
        for _ in range(loopC):
            random_bit_location = random.randint(0, len(data)-1)
            data = data[:random_bit_location] +  ['0','1'][random.randint(0,1)] + data[random_bit_location+1:]
        print("Injected error in data: ", data)
        return data




if __name__ == "__main__":
    server = Channel(host='127.0.0.1', port=8081)
    try:
        print("Socket Server[Channel] is starting....")
        server.start()
        server.startAcceptConnections()
    except KeyboardInterrupt:
        server.closeAllConnections()
        exit()
    except Exception as e:
        print(str(e))
        server.closeAllConnections()
        exit()
