#ifndef DRIVERS_SPI_H_
#define DRIVERS_SPI_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

class Spi
{
public:
  Spi();
  const eUSCI_SPI_MasterConfig spiDefaultConfig =
      {
          EUSCI_B_SPI_CLOCKSOURCE_SMCLK,                           // SMCLK Clock Source
          3000000,                                                 // SMCLK = DCO = 3MHZ
          500000,                                                  // SPICLK = 500khz
          EUSCI_B_SPI_MSB_FIRST,                                   // MSB First
          EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT, // Phase
          EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH,               // High polarity
          EUSCI_B_SPI_3PIN                                         // 3Wire SPI Mode
  };
  void TransmitData(uint8_t *write_buffer, uint8_t write_buffer_len, uint8_t cs_port, uint8_t cs_pin);
};

#endif /* DRIVERS_SPI_H_ */
