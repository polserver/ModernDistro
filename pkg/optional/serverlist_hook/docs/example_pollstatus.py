import socket;
import time;

print('Connecting...')
sock = socket.create_connection(('localhost',5003))
sock.settimeout(2);

seed = bytes([0x01, 0x02, 0x03, 0x04])
bts = bytes([0xF1, 0x00, 0x04, 0xFF])

sock.send(seed)
sock.send(bts)

while sock:
    rcv = sock.recv(1024)
    if (not rcv):
        break 
    print(rcv)
    
print('End')
