#include <calibration.h>

CalibrationMap TransmitCalibration1() {
    CalibrationMap calibration_1;
    calibration_1.emplace(0, CalibrationPoint{11.5, 3.299, 1.898});
    calibration_1.emplace(10, CalibrationPoint{11.5, 2.637, 1.509});
    calibration_1.emplace(20, CalibrationPoint{11.5, 1.866, 1.727});
    // TODO(dingbenjamin) : One more calibration point
    return calibration_1;
}

CalibrationMap TransmitCalibration2() {
    CalibrationMap calibration_2;
    calibration_2.emplace(0, CalibrationPoint{18.5, 3.494, 2.609});
    calibration_2.emplace(10, CalibrationPoint{18.5, 3.488, 2.347});
    calibration_2.emplace(20, CalibrationPoint{18.5, 3.419, 2.105});
    calibration_2.emplace(30, CalibrationPoint{18.5, 3.298, 1.897});
    return calibration_2;
}

CalibrationMap TransmitCalibration3() {
    CalibrationMap calibration_3;
    calibration_3.emplace(0, CalibrationPoint{17, 2.234, 1.536});
    calibration_3.emplace(10, CalibrationPoint{17, 2.504, 1.5});
    calibration_3.emplace(20, CalibrationPoint{17, 2.771, 1.537});
    calibration_3.emplace(30, CalibrationPoint{17, 2.997, 1.632});
    return calibration_3;
}

CalibrationMap TransmitCalibration4() {
    CalibrationMap calibration_4;
    calibration_4.emplace(0, CalibrationPoint{21, 3.485, 2.67});
    calibration_4.emplace(10, CalibrationPoint{21, 3.064, 3.326});
    calibration_4.emplace(20, CalibrationPoint{21, 2.29, 3.478});
    calibration_4.emplace(30, CalibrationPoint{21, 1.668, 3.055});
    return calibration_4;
}
