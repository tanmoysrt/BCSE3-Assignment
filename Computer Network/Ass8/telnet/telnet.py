import socket
import subprocess
from threading import Thread


# HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
# PORT = 5000  # Port to listen on (non-privileged ports are > 1023)

# sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# sock.bind((HOST, PORT))
# sock.listen()
# conn, addr = sock.accept()

# while True:
#     try:
#         data = conn.recv(1024)
#         res = subprocess.run(data.decode().strip(), shell=True, capture_output=True, cwd="../")
#         if res.returncode == 0:
#             conn.sendall(res.stdout)
#         else:
#             conn.sendall(res.stderr)
#         if not data:
#             break
#         if data.decode().strip() == "exit":
#             break
#     except:
#         break

# conn.close()
# sock.close()


class TelentServer:
    def __init__(self, host="127.0.0.1", port=5000):
        self.host = host
        self.port = port
        self.exited = False
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
                continue

        

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
