import socket
import subprocess
from time import sleep

def runInTerminal(command):
    data  = subprocess.run(command, shell=True, capture_output=True, cwd="./remote")
    if data.returncode == 0:
        return data.stdout.decode('utf-8')
    else:
        return data.stderr.decode('utf-8')

    
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("127.0.0.1", 6777))
s.listen(1)

while True:
    conn, addr = s.accept()
    while True:
        data = conn.recv(1024)
        data = data.decode()
        if data == "ls": response = runInTerminal(data)
        elif data == "pwd": response = runInTerminal(data)
        elif data.startswith("get"):
            # download file
            filename = data.split(" ")[1].strip()
            file = open("./remote/"+filename, "rb")
            d = file.read(1024)
            while d:
                conn.send(d)
                d = file.read(1024)
            file.close()
            sleep(1)
            conn.send("EOF".encode())
            sleep(1)
            response = "file_sent"
        elif data.startswith("put"):
            # upload file
            filename = data.split(" ")[1].strip()
            print(filename)
            file = open("./remote/"+filename, "wb")
            while True:
                data = conn.recv(1024)
                if data == b"EOF" : 
                    file.close()
                    break
                else: file.write(data)
            response = "upload_successful"
        else: response = "not_found"
        conn.sendall(response.encode())
    conn.close()