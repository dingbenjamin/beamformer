import util.uart as uart

conn = uart.Uart('/dev/ttyACM0', 9600)

conn.set_dac_voltage('dac_1', 'channel_b', 2.5)
