import socket
import struct
import sys

multicast_group = '224.3.29.71'
server_address = ('', 5007)

# Create the socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

sock.bind(server_address)

# Tell the operating system to add the socket to the multicast group
# on all interfaces.
group = socket.inet_aton(multicast_group)
mreq = struct.pack('4sL', group, socket.INADDR_ANY)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_ADD_MEMBERSHIP, mreq)

# Receive/respond loop
while True:
    print('waiting to receive message')
    data, address = sock.recvfrom(1024)
    
    print('received {} bytes from {}'.format(len(data), address))
    print(data)