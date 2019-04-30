import util.uart as uart
import time

conn = uart.Uart('/dev/ttyACM0', 9600)

# conn.set_dac_voltage('dac_1', 'channel_a', 1)
# time.sleep(1)
# conn.set_dac_voltage('dac_1', 'channel_b', 1)
# time.sleep(1)

conn.set_pa_attenuation('pa_1', 30)
