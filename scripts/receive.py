import serial
import csv 

uart = serial.Serial()
uart.baudrate = 921600
uart.port = '/dev/ttyACM0'
uart.open()

i = 0

while True:
    sync1 = uart.read(1)
    if int.from_bytes(sync1, byteorder='little') == 77:
        sync2 = uart.read(1)
        if int.from_bytes(sync2, byteorder='little') == 77:
            data = uart.read(16)
            i1 = int.from_bytes(data[0:2], byteorder='little') * 3.3 / 16384
            q1 = int.from_bytes(data[2:4], byteorder='little') * 3.3 / 16384
            i2 = int.from_bytes(data[4:6], byteorder='little') * 3.3 / 16384
            q2 = int.from_bytes(data[6:8], byteorder='little') * 3.3 / 16384
            i3 = int.from_bytes(data[8:10], byteorder='little') * 3.3 / 16384
            q3 = int.from_bytes(data[10:12], byteorder='little') * 3.3 / 16384
            i4 = int.from_bytes(data[12:14], byteorder='little') * 3.3 / 16384
            q4 = int.from_bytes(data[14:16], byteorder='little') * 3.3 / 16384


            with open('downsampled.csv', 'a') as f:
                data_writer = csv.writer(f, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
                data_writer.writerow([i1, q1, i2, q2, i3, q3, i4, q4])
                print('{}: {}, {}, {}, {}, {}, {}, {}, {}'.format(i, i1, q1, i2, q2, i3, q3, i4, q4))
                i += 1
    else:
        print("little")
        print(int.from_bytes(sync1, byteorder='little'))
        print("big")
        print(int.from_bytes(sync1, byteorder='big'))
        print("-------")


