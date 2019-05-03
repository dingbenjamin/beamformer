import util.uart as uart
import time

conn = uart.Uart('/dev/ttyACM0', 9600)

for i in range(0,180, 10):


conn.set_dac_voltage('dac_'+str(i+1), 'channel_a', 2.5)
time.sleep(1)
conn.set_dac_voltage('dac_'+str(i+1), 'channel_b', 2.5)
time.sleep(1)

# conn.set_pa_attenuation('pa_1', 0)
