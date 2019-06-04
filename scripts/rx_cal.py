import serial
import csv 

uart = serial.Serial()
uart.baudrate = 921600
uart.port = '/dev/ttyACM0'
uart.open()

Fs = 884 * 2
cal_length = Fs * 10

q4 = 0
i4 = 0
q3 = 0
q1 = 0
i3 = 0
q2 = 0
i2 = 0
i1 = 0

for i in range(cal_length):
    sync1 = uart.read(1)
    if int.from_bytes(sync1, byteorder='little') == 77:
        sync2 = uart.read(1)
        if int.from_bytes(sync2, byteorder='little') == 77:
            data = uart.read(16)
            q4 += int.from_bytes(data[0:2], byteorder='little') * 3.3 / 16384
            i4 += int.from_bytes(data[2:4], byteorder='little') * 3.3 / 16384
            q3 += int.from_bytes(data[4:6], byteorder='little') * 3.3 / 16384
            q1 += int.from_bytes(data[6:8], byteorder='little') * 3.3 / 16384
            i3 += int.from_bytes(data[8:10], byteorder='little') * 3.3 / 16384
            q2 += int.from_bytes(data[10:12], byteorder='little') * 3.3 / 16384
            i2 += int.from_bytes(data[12:14], byteorder='little') * 3.3 / 16384
            i1 += int.from_bytes(data[14:16], byteorder='little') * 3.3 / 16384

i1 = i1/cal_length
q1 = q1/cal_length
i2 = i2/cal_length
q2 = q2/cal_length
i3 = i3/cal_length
q3 = q3/cal_length
i4 = i4/cal_length
q4 = q4/cal_length

with open('rx_cal.csv', 'a') as f:
    data_writer = csv.writer(f, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    data_writer.writerow([i1, q1, i2, q2, i3, q3, i4, q4])
    print('Calibration: \t{}, \t{}, \t{}, \t{}, \t{}, \t{}, \t{}, \t{}'.format(round(i1, 2), round(q1, 2), round(i2, 2), round(q2, 2), round(i3, 2), round(q3, 2), round(i4, 2), round(q4, 2)))

