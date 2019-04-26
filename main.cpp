#include <drivers/dac.h>
#include <drivers/pa.h>
#include <drivers/uart.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "msp.h"

void main(void) {
  WDT_A_holdTimer();

  Uart uart;
  uint8_t uart_read_buffer[50];

  Spi spi;
  Dac dac1(&spi, GPIO_PORT_P2, GPIO_PIN6);
  PowerAmplifier pa1(&spi, GPIO_PORT_P2, GPIO_PIN7);
  Dac dac2(&spi, GPIO_PORT_P5, GPIO_PIN6);
  PowerAmplifier pa2(&spi, GPIO_PORT_P2, GPIO_PIN4);

    uint8_t command_id;

  while (1) {
    uart.Read(uart_read_buffer, 1);

    if(uart_read_buffer[0] != Uart::kSyncByte) {
        continue;
    }

    uart.Read(command_id, 1);

    if()

    uart.Read(uart_read_buffer, 1)
    uint8_t config_byte = 0x07;
    dac1.SetDacVoltage(2.5, config_byte);
    config_byte = 0x17;
    dac1.SetDacVoltage(2.5, config_byte);

    config_byte = 0x07;
    dac2.SetDacVoltage(2.5, config_byte);
    config_byte = 0x17;
    dac2.SetDacVoltage(2.5, config_byte);
  }
}
