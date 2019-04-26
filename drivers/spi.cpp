#include <drivers/spi.h>

Spi::Spi() {
  GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                                             GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7,
                                             GPIO_PRIMARY_MODULE_FUNCTION);

  SPI_initMaster(EUSCI_B0_BASE, &spiDefaultConfig);
  SPI_enableModule(EUSCI_B0_BASE);
}

void Spi::TransmitData(uint8_t *write_buffer, uint8_t write_buffer_len,
                       uint8_t cs_port, uint8_t cs_pin) {
  MAP_GPIO_setOutputLowOnPin(cs_port, cs_pin);
  for (uint8_t i = 0; i < write_buffer_len; i++) {
    SPI_transmitData(EUSCI_B0_BASE, write_buffer[i]);
  }
  MAP_GPIO_setOutputHighOnPin(cs_port, cs_pin);
}
