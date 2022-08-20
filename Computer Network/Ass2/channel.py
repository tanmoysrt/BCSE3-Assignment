from socketServer import SocketServer


class Channel(SocketServer):
    def __init__(self, host, port):
        super().__init__(host, port)
        # Update modifyData static method of  SocketServer with  Channel's
        SocketServer.modifyData = Channel.modifyData

    @staticmethod
    def modifyData(data):
        print(data)
        return data
    
    




tmp = Channel(host='127.0.0.1', port=8081)
tmp.start()
# tmp.startAcceptConnections()
SocketServer.modifyData('Hello')
