import time
from clientServer.socketServer import SocketServer
import random

class Channel(SocketServer):
    errorCount = 0
    def __init__(self, host, port):
        super().__init__(host, port)
        # Update modifyData static method of  SocketServer with  Channel's
        SocketServer.modifyData = Channel.modifyData

    @staticmethod
    def modifyData(data):
        # return data
        # return data
        if data in ["disconnect:"]:
            return data
        return Channel.injectErrorInData(data, loopC=1)
    
    @staticmethod
    def injectErrorInData(data, loopC=5):
        # time.sleep(random.random())
        olddata = data
        for _ in range(loopC):
            random_bit_location = random.randint(0, len(data)-1)
            should_inject_error = Channel.errorCount % 17 == 0
            Channel.errorCount += 1
            if should_inject_error:
                data = data[:random_bit_location] +  ("0" if data[random_bit_location] == "1" else "1") + data[random_bit_location+1:]
        if olddata != data:
            print("Injected Error : "+olddata+" ---> "+data)
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
