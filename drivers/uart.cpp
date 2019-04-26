#include <drivers/uart.h>

Uart::Uart() {
  /* Selecting P1.2 and P1.3 in UART mode */
  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
      GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

  /* Setting DCO to 12MHz */
  CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

  //![Simple UART Example]
  /* Configuring UART Module */
  MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

  /* Enable UART module */
  MAP_UART_enableModule(EUSCI_A0_BASE);
}

void Uart::Read(uint8_t *read_buffer, uint8_t read_buffer_len) {
    for(uint8_t i = 0; i < read_buffer_len; i++) {
        read_buffer[i] = MAP_UART_receiveData(EUSCI_A0_BASE);
    }
}
