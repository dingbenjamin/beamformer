#include <commands/command.h>
#include <drivers/dac.h>
#include <drivers/pa.h>
#include <drivers/uart.h>
#include <nanopb/pb_decode.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <cassert>
#include "msp.h"

void main(void) {
    WDT_A_holdTimer();

    Uart uart;
    uint8_t uart_read_buffer[100];

    const Spi spi;
    Dac dac1(spi, GPIO_PORT_P2, GPIO_PIN6);
    PowerAmplifier pa1(spi, GPIO_PORT_P2, GPIO_PIN4);
    Dac dac2(spi, GPIO_PORT_P5, GPIO_PIN6);
    PowerAmplifier pa2(spi, GPIO_PORT_P6, GPIO_PIN6);
    Dac dac3(spi, GPIO_PORT_P6, GPIO_PIN7);
    PowerAmplifier pa3(spi, GPIO_PORT_P2, GPIO_PIN3);
    Dac dac4(spi, GPIO_PORT_P5, GPIO_PIN1);
    PowerAmplifier pa4(spi, GPIO_PORT_P3, GPIO_PIN5);

    uint8_t command_id;

    while (1) {
        uart.Read(uart_read_buffer, 1);

        if (uart_read_buffer[0] != Uart::kSyncByte) {
            continue;
        }

        uart.Read(&command_id, 1);

        switch (command_id) {
            case kSetDacVoltageCommand:
                uart.Read(uart_read_buffer, SetDacVoltagePayload_size);
                SetDacVoltagePayload dac_payload;
                pb_istream_t stream = pb_istream_from_buffer(
                    uart_read_buffer, SetDacVoltagePayload_size);
                assert(
                    pb_decode(&stream, SetDacVoltagePayload_fields, &dac_payload));

                uint8_t config_byte = (dac_payload.dac_channel == Dac::kDacChannelA)
                                          ? Dac::kDefaultConfigByteChannelA
                                          : Dac::kDefaultConfigByteChannelB;

               
                if (dac_payload.dac_id == 0) {
                    dac1.SetDacVoltage(dac_payload.voltage, config_byte);
                } else if(dac_payload.dac_id == 1){
                    dac2.SetDacVoltage(dac_payload.voltage, config_byte);
                } else if(dac_payload.dac_id == 2){
                    dac3.SetDacVoltage(dac_payload.voltage, config_byte);
                } else {
                    dac4.SetDacVoltage(dac_payload.voltage, config_byte);
                }

            case kSetPaAttenuationCommand:
                uart.Read(uart_read_buffer, SetPaAttenuationPayload_size);
                SetPaAttenuationPayload pa_payload;
                stream = pb_istream_from_buffer(
                    uart_read_buffer, SetPaAttenuationPayload_size);
                assert(pb_decode(&stream, SetPaAttenuationPayload_fields,
                                 &pa_payload));

                if (pa_payload.pa_id == 0) {
                    pa1.SetPaAttenuation(pa_payload.attenuation);
                } else if (pa_payload.pa_id == 1){
                    pa2.SetPaAttenuation(pa_payload.attenuation);
                } else if (pa_payload.pa_id == 2) {
                    pa3.SetPaAttenuation(pa_payload.attenuation);
                } else{
                    pa4.SetPaAttenuation(pa_payload.attenuation);
                }
        }
    }
}
