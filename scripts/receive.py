import serial
import csv 

uart = serial.Serial()
uart.baudrate = 921600
uart.port = '/dev/ttyACM0'
uart.open()

FIXED_POINT_SCALING = 2**32 / 5

i = 0

while True:
    data = uart.read(32)
    i1 = int.from_bytes(data[0:2], byteorder='little')
    q1 = int.from_bytes(data[2:4], byteorder='little')
    i2 = int.from_bytes(data[4:6], byteorder='little')
    q2 = int.from_bytes(data[6:8], byteorder='little')
    i3 = int.from_bytes(data[8:10], byteorder='little')
    q3 = int.from_bytes(data[10:12], byteorder='little')
    i4 = int.from_bytes(data[12:14], byteorder='little')
    q4 = int.from_bytes(data[14:16], byteorder='little')

    with open('downsampled.csv', 'a') as f:
        data_writer = csv.writer(f, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
        data_writer.writerow([i1, q1, i2, q2, i3, q3, i4, q4])
        print('{}: {}, {}, {}, {}, {}, {}, {}, {}'.format(i, i1, q1, i2, q2, i3, q3, i4, q4))
        i += 1


