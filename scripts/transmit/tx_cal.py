import util.uart as uart
import time
import math

conn = uart.Uart('/dev/ttyACM0', 9600)

start_angle = 170

conn.set_dac_voltage('dac_4', 'channel_a', 2.5)
time.sleep(1)
conn.set_dac_voltage('dac_4', 'channel_b', 2.5)
time.sleep(1)

# Steps phase of IQ modulator in 10 degree steps on a circle
# of magnitude 1.
for i in range(start_angle, 370, 10):
    I = math.cos(math.radians(i)) +2.5
    Q = math.sin(math.radians(i))+2.5
    print("Angle: "+str(i))
    print("I= "+str(I))
    print("Q= "+str(Q))

    conn.set_dac_voltage('dac_4', 'channel_a', I)
    time.sleep(1)
    conn.set_dac_voltage('dac_4', 'channel_b', Q)
    time.sleep(1)