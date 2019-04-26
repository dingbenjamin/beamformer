import util.uart as uart

conn = uart.Uart('/dev/ttyACM0', 9600)

conn.set_dac_voltage(1, 'a', 2.5)

b = conn.ser.read(1)

print(b)