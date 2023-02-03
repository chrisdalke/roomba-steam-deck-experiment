import socket
import time
import serial
ser = serial.Serial('/dev/ttyACM1', 115200)

def write_arr(arr):
    ser.write(bytes([arr]))

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

    if input_line == "reset":
        write_arr([7])

    if input_line == "drive_start":
        write_arr([146, 255, 128, 255, 128])

    if input_line == "drive_stop":
        write_arr([146, 0,0,0,0])