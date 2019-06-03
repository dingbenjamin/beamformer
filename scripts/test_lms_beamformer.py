# Simulate online beamforming using data generated from collectPlanWave(...), a function
# from MATLAB's phased array toolbox.

import csv
from beamformer import lms_beamformer
import numpy as np
import matplotlib.pyplot as plt
from scipy import signal
from pyargus.antennaArrayPattern import array_rad_pattern_plot

with open("scripts/tests/simulated_receive_data_-ve20.csv") as f:
    reader = csv.reader(f)
    data = []
    for row in reader:
        data.append([complex(num.replace("i","j",1)) for num in row])

rx = np.array(data)

# Create training signal
fm = 10e3
Fs = 5*fm
t = np.arange(0, 1,1/Fs)
training_signal = np.cos(np.deg2rad(2*np.pi*fm*t))

num_elements = 4

beamformer = lms_beamformer.LmsBeamformer(num_elements,training_signal, 'adaptive')
estimated_signal = np.array([])
with open("scripts/tests/estimated_signal.csv",'w') as f1, open("scripts/tests/snapshots.csv",'w') as f2:
    data_writer1 = csv.writer(f1, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    data_writer2 = csv.writer(f2, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)
    for snapshot in rx:
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


