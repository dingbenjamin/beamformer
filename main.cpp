#include <commands/command.h>
#include <drivers/dac.h>
#include <drivers/pa.h>
#include <drivers/uart.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "msp.h"
#include <nanopb/pb_decode.h>
#include <cassert>

void main(void)
{
    WDT_A_holdTimer();

    Uart uart;
    uint8_t uart_read_buffer[100];

    Spi spi;
    Dac dac1(&spi, GPIO_PORT_P2, GPIO_PIN6);
    PowerAmplifier pa1(&spi, GPIO_PORT_P2, GPIO_PIN7);
    Dac dac2(&spi, GPIO_PORT_P5, GPIO_PIN6);
    PowerAmplifier pa2(&spi, GPIO_PORT_P2, GPIO_PIN4);

    uint8_t command_id;

    while (1)
    {
        uart.Read(uart_read_buffer, 1);

        if (uart_read_buffer[0] != Uart::kSyncByte)
        {
            continue;
        }

        uart.Read(&command_id, 1);

        if (command_id == kSetDacVoltageCommand)
        {
            uart.Read(uart_read_buffer, SetDacVoltagePayload_size);
            SetDacVoltagePayload payload;
            pb_istream_t stream = pb_istream_from_buffer(
                    uart_read_buffer, SetDacVoltagePayload_size);
            assert(pb_decode(&stream, SetDacVoltagePayload_fields, &payload));


            uint8_t config_byte =
                    (payload.dac_channel == Dac::kDacChannelA) ?
                            Dac::kDefaultConfigByteChannelA :
                            Dac::kDefaultConfigByteChannelB;

            if (payload.dac_id == 0)
            {
                dac1.SetDacVoltage(payload.voltage, config_byte);
            }
            else
            {
                dac2.SetDacVoltage(2.5, config_byte);
            }
        }
    }
}
