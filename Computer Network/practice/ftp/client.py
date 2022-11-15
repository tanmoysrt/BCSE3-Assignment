import socket
from time import sleep

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 6777))

while True:
    msg = input("enter cmd : ")
    s.sendall(msg.encode())
    if msg.startswith("get"):
        sleep(1)
        # download file
        filename = msg.split(" ")[1].strip()
        file = open("./local/"+filename, "wb")
        while True:
            d = s.recv(1024)
            if d == b"EOF":
                file.close()
                break
            else:
                file.write(d)
    elif msg.startswith("put"):
        sleep(1)
        # upload file
        filename = msg.split(" ")[1].strip()
        file = open("./local/"+filename, "rb")
        d = file.read(1024)
        while d:
            s.send(d)
            d = file.read(1024)
        file.close()
        sleep(1)
        s.send("EOF".encode())
        
    msg = s.recv(1024)
    print(msg.decode())
    sleep(1)