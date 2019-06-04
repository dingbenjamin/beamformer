# Simulate online beamforming using data generated from collectPlanWave(...), a function
# from MATLAB's phased array toolbox.

import csv
from beamformer import lms_beamformer
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
from pyargus.antennaArrayPattern import array_rad_pattern_plot
import serial

uart = serial.Serial()
uart.baudrate = 921600
uart.port = '/dev/ttyACM0'
uart.open()

# Create training signal
fm = 200
Fs = 884*2
t = np.arange(0, 1,1/Fs)
training_signal = np.cos(np.deg2rad(2*np.pi*fm*t))

num_elements = 4

beamformer = lms_beamformer.LmsBeamformer(num_elements,training_signal, 'adaptive')
estimated_signal = np.array([])
with open("scripts/tests/estimated_signal_1.csv",'w') as f1, open("scripts/tests/snapshots_1.csv",'w') as f2:
    data_writer1 = csv.writer(f1, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    data_writer2 = csv.writer(f2, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    while True:
        sync1 = uart.read(1)
        if int.from_bytes(sync1, byteorder='little') == 77:
            sync2 = uart.read(1)
            if int.from_bytes(sync2, byteorder='little') == 77:
                data = uart.read(16)
                q4 = int.from_bytes(data[0:2], byteorder='little') * 3.3 / 16384
                i4 = int.from_bytes(data[2:4], byteorder='little') * 3.3 / 16384
                q3 = int.from_bytes(data[4:6], byteorder='little') * 3.3 / 16384
                q1 = int.from_bytes(data[6:8], byteorder='little') * 3.3 / 16384
                i3 = int.from_bytes(data[8:10], byteorder='little') * 3.3 / 16384
                q2 = int.from_bytes(data[10:12], byteorder='little') * 3.3 / 16384
                i2 = int.from_bytes(data[12:14], byteorder='little') * 3.3 / 16384
                i1 = int.from_bytes(data[14:16], byteorder='little') * 3.3 / 16384
        
                snapshot = np.array([complex(i1, q1), complex(i2,q2), complex(i3,q3), complex(i4,q4)])
                (w, y) = beamformer.update_weights(snapshot.reshape(num_elements,1))
                estimated_signal = np.append(estimated_signal, y)
                data_writer1.writerow(y[0])
                data_writer2.writerow([num for num in snapshot])

print("Estimated weights: "+str(w))
plt.figure(1)
plt.plot(estimated_signal.real)

# Antenna element positions for ULA
fc = 920e6
vp = 3e8
wavelength = vp/fc
spacing = wavelength/2
y_coords = np.arange(num_elements)*spacing - (num_elements-1)*spacing/2
x_coords = np.zeros(num_elements)
array_alignment = np.array((x_coords, y_coords))
    
array_rad_pattern_plot(w=w.reshape(num_elements,), array_alignment = array_alignment)
plt.show()


