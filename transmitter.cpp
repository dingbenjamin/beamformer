#include <transmitter.h>

Transmitter::Transmitter(const Spi& spi, uint8_t dac_cs_port,
                         uint8_t dac_cs_pin, uint8_t pa_cs_port,
                         uint8_t pa_cs_pin, CalibrationMap calibration_map)
    : dac{spi, dac_cs_port, dac_cs_pin},
      power_amplifier{spi, pa_cs_port, pa_cs_pin},
      calibration{calibration_map} {
    // TODO(dingbenjamin) : Use move semantics for the calibration map
}

bool Transmitter::Steer(uint16_t angle) {
    CalibrationMap::const_iterator it = calibration.find(angle);
    if (it == calibration.end()) {
        return false;
    }
    else {
        power_amplifier.SetPaAttenuation(calibration[angle].pa_attenuation);
        dac.SetDacVoltage(calibration[angle].i_voltage, Dac::kDefaultConfigByteChannelA);
        dac.SetDacVoltage(calibration[angle].q_voltage, Dac::kDefaultConfigByteChannelB);
        return true;
    }
}

void Transmitter::SetDacVoltage(float value, uint8_t config_byte) {
    dac.SetDacVoltage(value, config_byte);
}
    
void Transmitter::SetPaAttenuation(float attenuation) {
    power_amplifier.SetPaAttenuation(attenuation);
}
