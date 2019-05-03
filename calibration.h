#ifndef CALIBRATION_H_
#define CALIBRATION_H_

#include <map>

struct CalibrationPoint {
    float pa_attenuation{0};
    float i_voltage{0};
    float q_voltage{0};
};

// Use a map here instead of compile time array for less memory usage at
// lower quantities of calibration points
using CalibrationMap = std::map<uint16_t, CalibrationPoint>;

CalibrationMap TransmitCalibration1();
CalibrationMap TransmitCalibration2();
CalibrationMap TransmitCalibration3();
CalibrationMap TransmitCalibration4();

#endif /* CALIBRATION_H */
