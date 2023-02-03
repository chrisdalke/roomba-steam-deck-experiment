import socket
import time
import serial

MCAST_GRP = '224.3.29.71'
MCAST_PORT = 5007
MULTICAST_TTL = 2

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sock.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, MULTICAST_TTL)

def write_arr(arr):
    sock.sendto(bytes(arr), (MCAST_GRP, MCAST_PORT))


while True:
    input_line = input('>').rstrip()

    if input_line == "start":
        write_arr([128])

    if input_line == "safe":
        write_arr([131])

    if input_line == "passive":
        write_arr([132])

    if input_line == "full":
        write_arr([132])

    if input_line == "stop":
        write_arr([173])
    if input_line == "dock":
        write_arr([143])


    if input_line == "clean":
        write_arr([135])

    if input_line == "reset":
        write_arr([7])

    if input_line == "drive_start":
        write_arr([146, 255, 128, 255, 128])

    if input_line == "drive_stop":
        write_arr([146, 0, 0, 0, 0])