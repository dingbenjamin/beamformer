import serial
import csv 

uart = serial.Serial()
uart.baudrate = 460800
uart.port = '/dev/ttyACM0'
uart.timeout = 1
uart.open()

FIXED_POINT_SCALING = 2**32 / 5

i = 0

while True:
    data = uart.read(32)
    i1 = int.from_bytes(data[0:4], byteorder='little') / FIXED_POINT_SCALING
    q1 = int.from_bytes(data[4:8], byteorder='little') / FIXED_POINT_SCALING
    i2 = int.from_bytes(data[8:12], byteorder='little') / FIXED_POINT_SCALING
    q2 = int.from_bytes(data[12:16], byteorder='little') / FIXED_POINT_SCALING
    i3 = int.from_bytes(data[16:20], byteorder='little') / FIXED_POINT_SCALING
    q3 = int.from_bytes(data[20:24], byteorder='little') / FIXED_POINT_SCALING
    i4 = int.from_bytes(data[24:28], byteorder='little') / FIXED_POINT_SCALING
    q4 = int.from_bytes(data[28:32], byteorder='little') / FIXED_POINT_SCALING

    with open('downsampled.csv', 'a') as f:
        data_writer = csv.writer(f, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
        data_writer.writerow([i1, q1, i2, q2, i3, q3, i4, q4])
        print('{}: {}, {}, {}, {}, {}, {}, {}, {}'.format(i, i1, q1, i2, q2, i3, q3, i4, q4))
        i += 1


