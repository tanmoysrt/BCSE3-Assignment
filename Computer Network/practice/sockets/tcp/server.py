import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("127.0.0.1", 6777))
s.listen(1)

while True:
    conn, addr = s.accept()
    data = conn.recv(1024)
    data = data.decode()
    conn.sendall(f"[ECHO] {data}".encode())
    conn.close()