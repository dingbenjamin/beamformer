#ifndef TRANSMITTER_H_
#define TRANSMITTER_H_

#include <calibration.h>
#include <drivers/dac.h>
#include <drivers/pa.h>
#include <drivers/spi.h>
#include <stdbool.h>
#include <stdint.h>

class Transmitter {
   public:
    Transmitter(const Spi& spi, uint8_t dac_cs_port, uint8_t dac_cs_pin,
                uint8_t pa_cs_port, uint8_t pa_cs_pin,
                CalibrationMap calibration_map);
    // TODO:(dingbenjamin): Angle data type
    bool Steer(uint16_t angle);

    // Convenient passthroughs for calibration and such
    void SetDacVoltage(float value, uint8_t config_byte);
    void SetPaAttenuation(float attenuation);

   private:
    Dac dac;
    PowerAmplifier power_amplifier;
    CalibrationMap calibration;
    // TODO:(dingbenjamin): Const this
};

#endif /* TRANSMITTER_H */
