#include <drivers/pa.h>

PowerAmplifier::PowerAmplifier(Spi *spi, uint8_t cs_port, uint8_t cs_pin)
    : spi(spi), cs_port(cs_port), cs_pin(cs_pin) {
    MAP_GPIO_setAsOutputPin(cs_port, cs_pin);
    MAP_GPIO_setOutputHighOnPin(cs_port, cs_pin);

    SetPaAttenuation(30);
}

void PowerAmplifier::SetPaAttenuation(float attenuation) {
    uint8_t write_command = GetGainCode(attenuation);
    spi->TransmitData(&write_command, 1, cs_port, cs_pin);
}

uint8_t PowerAmplifier::GetGainCode(float attenuation) {
    if (attenuation > kPaMaxAttenuation) {
        attenuation = kPaMaxAttenuation;
    }

    if (attenuation < kPaMinAttenuation) {
        attenuation = kPaMinAttenuation;
    }
    uint8_t code = attenuation / kPaMaxAttenuation * kPaResolution;
    return code;
}
