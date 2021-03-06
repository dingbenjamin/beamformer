import serial
import csv 

uart = serial.Serial()
uart.baudrate = 921600
uart.port = '/dev/ttyACM0'
uart.open()

i = 0

# Import calibration data for DC offsets
with open('rx_cal.csv', newline='') as rx_cal_file:
    rx_cal = [float(x) for x in  list(csv.reader(rx_cal_file))[0]]

while True:
    sync1 = uart.read(1)
    if int.from_bytes(sync1, byteorder='little') == 77:
        sync2 = uart.read(1)
        if int.from_bytes(sync2, byteorder='little') == 77:
            data = uart.read(16)
            q4 = int.from_bytes(data[0:2], byteorder='little') * 3.3 / 16384 - rx_cal[7]
            i4 = int.from_bytes(data[2:4], byteorder='little') * 3.3 / 16384 - rx_cal[6]
            q3 = int.from_bytes(data[4:6], byteorder='little') * 3.3 / 16384 - rx_cal[5]
            q1 = int.from_bytes(data[6:8], byteorder='little') * 3.3 / 16384 - rx_cal[1]
            i3 = int.from_bytes(data[8:10], byteorder='little') * 3.3 / 16384 - rx_cal[4]
            q2 = int.from_bytes(data[10:12], byteorder='little') * 3.3 / 16384 - rx_cal[3]
            i2 = int.from_bytes(data[12:14], byteorder='little') * 3.3 / 16384 - rx_cal[2]
            i1 = int.from_bytes(data[14:16], byteorder='little') * 3.3 / 16384 - rx_cal[0]


            with open('downsampled.csv', 'a') as f:
                # data_writer = csv.writer(f, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
                # data_writer.writerow([i1, q1, i2, q2, i3, q3, i4, q4])
                print('{}: \t{}, \t{}, \t{}, \t{}, \t{}, \t{}, \t{}, \t{}'.format(i, round(i1, 3), round(q1, 3), round(i2, 3), round(q2, 3), round(i3, 3), round(q3, 3), round(i4, 3), round(q4, 3)))
                i += 1

