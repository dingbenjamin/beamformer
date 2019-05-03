#include <commands/command.h>
#include <drivers/dac.h>
#include <drivers/pa.h>
#include <drivers/uart.h>
#include <nanopb/pb_decode.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <transmitter.h>
#include <cassert>
#include "msp.h"

void main(void) {
    WDT_A_holdTimer();

    Uart uart;
    uint8_t uart_read_buffer[100];

    const Spi spi;

    Transmitter chain_1{spi,          GPIO_PORT_P2, GPIO_PIN6,
                        GPIO_PORT_P2, GPIO_PIN4,    TransmitCalibration1()};
    Transmitter chain_2{spi,          GPIO_PORT_P5, GPIO_PIN6,
                        GPIO_PORT_P6, GPIO_PIN6,    TransmitCalibration2()};
    Transmitter chain_3{spi,          GPIO_PORT_P6, GPIO_PIN7,
                        GPIO_PORT_P2, GPIO_PIN3,    TransmitCalibration3()};
    Transmitter chain_4{spi,          GPIO_PORT_P5, GPIO_PIN1,
                        GPIO_PORT_P3, GPIO_PIN5,    TransmitCalibration4()};

    uint8_t command_id;

    while (1) {
        uart.Read(uart_read_buffer, 1);

        if (uart_read_buffer[0] != Uart::kSyncByte) {
            continue;
        }

        uart.Read(&command_id, 1);

        switch (command_id) {
            case kSteerCommand:
                uart.Read(uart_read_buffer, SetSteeringAnglePayload_size);
                SetSteeringAnglePayload steer_payload;
                pb_istream_t stream = pb_istream_from_buffer(
                    uart_read_buffer, SetSteeringAnglePayload_size);
                assert(pb_decode(&stream, SetSteeringAnglePayload_fields,
                                 &steer_payload));
                chain_1.Steer(steer_payload.steering_angle);
                chain_2.Steer(steer_payload.steering_angle);
                chain_3.Steer(steer_payload.steering_angle);
                chain_3.Steer(steer_payload.steering_angle);
                // TODO(dingbenjamin): Error handling for invalid angle
                break;

            case kSetDacVoltageCommand:
                uart.Read(uart_read_buffer, SetDacVoltagePayload_size);
                SetDacVoltagePayload dac_payload;
                stream = pb_istream_from_buffer(
                    uart_read_buffer, SetDacVoltagePayload_size);
                assert(pb_decode(&stream, SetDacVoltagePayload_fields,
                                 &dac_payload));

                uint8_t config_byte =
                    (dac_payload.dac_channel == Dac::kDacChannelA)
                        ? Dac::kDefaultConfigByteChannelA
                        : Dac::kDefaultConfigByteChannelB;

                if (dac_payload.dac_id == 0) {
                    chain_1.SetDacVoltage(dac_payload.voltage, config_byte);
                } else if (dac_payload.dac_id == 1) {
                    chain_2.SetDacVoltage(dac_payload.voltage, config_byte);
                } else if (dac_payload.dac_id == 2) {
                    chain_3.SetDacVoltage(dac_payload.voltage, config_byte);
                } else if (dac_payload.dac_id == 3) {
                    chain_4.SetDacVoltage(dac_payload.voltage, config_byte);
                } else {
                    // TODO(dingbenjamin): Flag an error here
                }
                break;

            case kSetPaAttenuationCommand:
                uart.Read(uart_read_buffer, SetPaAttenuationPayload_size);
                SetPaAttenuationPayload pa_payload;
                stream = pb_istream_from_buffer(uart_read_buffer,
                                                SetPaAttenuationPayload_size);
                assert(pb_decode(&stream, SetPaAttenuationPayload_fields,
                                 &pa_payload));

                if (pa_payload.pa_id == 0) {
                    chain_1.SetPaAttenuation(pa_payload.attenuation);
                } else if (pa_payload.pa_id == 1) {
                    chain_2.SetPaAttenuation(pa_payload.attenuation);
                } else if (pa_payload.pa_id == 2) {
                    chain_3.SetPaAttenuation(pa_payload.attenuation);
                } else if (pa_payload.pa_id == 3) {
                    chain_4.SetPaAttenuation(pa_payload.attenuation);
                } else {
                    // TODO(dingbenjamin): Flag an error here
                }
                break;
        }
    }
}
