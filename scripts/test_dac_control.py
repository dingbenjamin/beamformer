import util.uart as uart

conn = uart.Uart('/dev/ttyACM0', 9600)

conn.set_dac_voltage('dac_2', 'channel_b', 2.5)

# conn.set_pa_attenuation('pa_2', 30)
