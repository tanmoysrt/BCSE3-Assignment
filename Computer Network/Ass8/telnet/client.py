
import socket
import subprocess

HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 5000  # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        while True:
            try:
                data = conn.recv(1024)
                # print(data.decode())
                res = subprocess.run(data.decode().strip(), shell=True, capture_output=True)
                if res.returncode == 0:
                    conn.sendall(res.stdout)
                else:
                    conn.sendall(res.stderr)
                if not data:
                    break
                if data.decode().strip() == "exit":
                    break
            except:
                break
    conn.close()
    s.close()