#ifndef DRIVERS_PA_H_
#define DRIVERS_PA_H_

#include <drivers/spi.h>
#include <stdbool.h>
#include <stdint.h>

class PowerAmplifier {
   public:
    PowerAmplifier(Spi *spi, uint8_t cs_port, uint8_t cs_pin);
    void SetPaAttenuation(float attenuation);
    uint8_t GetGainCode(float attenuation);

   private:
    Spi *spi;
    uint8_t cs_port;
    uint8_t cs_pin;
    static constexpr float kPaMaxAttenuation = 31.5;
    static constexpr float kPaMinAttenuation = 31.5;
    static constexpr uint16_t kPaResolution = 63;
};
#endif /* DRIVERS_PA_H_ */
