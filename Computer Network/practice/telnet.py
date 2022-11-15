import socket
import subprocess
from threading import Thread
from time import sleep

class TelentServer:
    def __init__(self, host="127.0.0.1", port=5000):
        self.host = host
        self.port = port
        self.exited = False
        self.exitedListening = False
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.bind((self.host, self.port))
        self.sock.settimeout(2)
        self.sock.listen()
        self.threads_pool = []
        print("Server is listening on port", port)

    def startProcess(self):
        while True:
            try:
                conn, addr = self.sock.accept()
                conn.settimeout(2)
                print("Connected by", addr)
                t = Thread(target=self.handleClient, args=(conn, addr))
                t.start()
                self.threads_pool.append(t)
            except socket.timeout:
                if self.exited:
                    break
                else : 
                    continue
            except:
                break
        self.exitedListening = True
        

    def handleClient(self, conn:socket.socket, addr):
        while True:
            try:
                data = conn.recv(1024)
                res = subprocess.run(data.decode().strip(), shell=True, capture_output=True)
                if res.returncode == 0:
                    conn.sendall(res.stdout)
                else:
                    conn.sendall(res.stderr)
                if not data:
                    break
                if data.decode().strip() == "exit":
                    break
            except socket.timeout:
                if self.exited:
                    break
                continue
            except:
                break
        conn.close()

    def stopProcess(self):
        self.exited = True
        while not self.exitedListening:
            sleep(1)
        for t in self.threads_pool:
            try:
                t.join()
            except:
                pass
        self.sock.close()

if __name__ == "__main__":
    server = TelentServer()

    try:
        server.startProcess()
    except KeyboardInterrupt:
        server.exited = True
        server.stopProcess()
        print("Server is closed")

