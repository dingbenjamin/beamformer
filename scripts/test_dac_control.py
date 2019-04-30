import util.uart as uart
import time

conn = uart.Uart('/dev/ttyACM0', 9600)

conn.set_dac_voltage('dac_1', 'channel_a', 0.5)
time.sleep(1)
conn.set_dac_voltage('dac_1', 'channel_b', 4.5)

# conn.set_pa_attenuation('pa_2', 30)
