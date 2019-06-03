#ifndef DRIVERS_UART_H_
#define DRIVERS_UART_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

class Uart {
   public:
    Uart();

    // Settings for 9600 baud
    const eUSCI_UART_Config uartConfig = {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,                // SMCLK Clock Source
        13,
        0,
        0,
        EUSCI_A_UART_NO_PARITY,                        // No Parity
        EUSCI_A_UART_LSB_FIRST,                        // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,                     // One stop bit
        EUSCI_A_UART_MODE,                             // UART mode
        EUSCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION
    };

    void Read(uint8_t *read_buffer, uint8_t read_buffer_len);
    void Write(uint8_t *write_buffer, uint8_t write_buffer_len);
    static constexpr uint8_t kSyncByte = 0xA5;
};

#endif /* DRIVERS_UART_H_ */
