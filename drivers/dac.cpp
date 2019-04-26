#include <drivers/dac.h>
#include <drivers/spi.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

Dac::Dac(Spi *spi, uint8_t cs_port, uint8_t cs_pin) : spi(spi), cs_port(cs_port), cs_pin(cs_pin) {
    MAP_GPIO_setAsOutputPin(cs_port, cs_pin);
    MAP_GPIO_setOutputHighOnPin(cs_port, cs_pin);

    // Initialise DAC to 2.5V (I=0, Q=0)
    uint8_t config_byte = GetConfigByte(kDacChannelA, kBuffered, kGain1, kActive);
    SetDacVoltage(kDacStartupVoltage, config_byte);
    config_byte = GetConfigByte(kDacChannelB, kBuffered, kGain1, kActive);
    SetDacVoltage(kDacStartupVoltage, config_byte);
}
    
void Dac::SetDacVoltage(float voltage, uint8_t config_byte) {
    uint16_t write_command = GetWriteCommand(voltage, config_byte);
    uint8_t write_buffer[2];
    write_buffer[1] = write_command;
    write_buffer[0] = write_command >> 8;
    spi->TransmitData(write_buffer, 2, cs_port, cs_pin);
}
uint16_t Dac::GetWriteCommand(float voltage, uint8_t config_bytes){
    uint16_t voltage_bytes = voltage / kVref * kDacResolution;
    uint16_t write_command = voltage_bytes << 2;
    write_command = write_command | (config_bytes << 12);
    return write_command;
}
uint8_t Dac::GetConfigByte(DacChannel channel, VrefBuffer buffered, DacVrefGain gain, DacShutdown active) {
    return (channel << 3) | (buffered << 2) | (gain << 1) | (active << 0);
}
