#ifndef DRIVERS_DAC_H_
#define DRIVERS_DAC_H_

#include <drivers/spi.h>
#include <stdint.h>
#include <stdbool.h>

class Dac
{
  public:
    Dac(Spi *spi, uint8_t cs_port, uint8_t cs_pin);
    void SetDacVoltage(float value, uint8_t config_byte);

    typedef enum
    {
        kDacChannelA = 0x00,
        kDacChannelB = 0x01,
    } DacChannel;
    typedef enum
    {
        kBuffered = 0x01,
        kUnbuffered = 0x00,
    } VrefBuffer;
    typedef enum
    {
        kGain1 = 0x01,
        kGain2 = 0x00,
    } DacVrefGain;
    typedef enum
    {
        kActive = 0x01,
        kShutdown = 0x00
    } DacShutdown;

    static constexpr uint8_t kDefaultConfigByteChannelA = 0x07;
    static constexpr uint8_t kDefaultConfigByteChannelB = 0x0F;


  private:
    uint16_t GetWriteCommand(float voltage, uint8_t config_bytes);
    uint8_t GetConfigByte(DacChannel channel, VrefBuffer buffered, DacVrefGain gain, DacShutdown active);
    static constexpr float kVref = 5;
    static constexpr float kDacStartupVoltage = 2.5;
    static constexpr uint16_t kDacResolution = 1023;
    Spi *spi;
    uint8_t cs_port;
    uint8_t cs_pin;
};

#endif /* DRIVERS_DAC_H_ */
