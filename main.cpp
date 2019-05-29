#include <commands/command.h>
#include <drivers/adc.h>
#include <drivers/dac.h>
#include <drivers/pa.h>
#include <drivers/uart.h>
#include <filter/filter_coefficients.h>
#include <filter/filters.h>
#include <nanopb/pb_decode.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <transmitter.h>
#include <cassert>
#include <functional>
#include <vector>
#include "msp.h"

void main(void) {
    WDT_A_holdTimer();

#ifdef TRANSMIT
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
                chain_4.Steer(steer_payload.steering_angle);
                // TODO(dingbenjamin): Error handling for invalid angle
                break;

            case kSetDacVoltageCommand:
                uart.Read(uart_read_buffer, SetDacVoltagePayload_size);
                SetDacVoltagePayload dac_payload;
                stream = pb_istream_from_buffer(uart_read_buffer,
                                                SetDacVoltagePayload_size);
                assert(pb_decode(&stream, SetDacVoltagePayload_fields,
                                 &dac_payload));
                WDT_A_holdTimer();
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
#endif

#ifdef RECEIVE
    Iir3 filter_1{Iir3::MakeIir3(iir_stg1_num_1, iir_stg1_den_1)};
    DownSampler<decimation_factor_stg1> downsampler_1{};

    Iir3 filter_2{Iir3::MakeIir3(iir_stg2_num_1, iir_stg2_den_1)};
    DownSampler<decimation_factor_stg2> downsampler_2{};

    Iir3 filter_3_1{Iir3::MakeIir3(iir_stg3_num_1, iir_stg3_den_1)};
    Iir3 filter_3_2{Iir3::MakeIir3(iir_stg3_num_2, iir_stg3_den_2)};
    Iir3 filter_3_3{Iir3::MakeIir3(iir_stg3_num_3, iir_stg3_den_3)};
    Gain gain_3{iir_stg3_gain};
    DownSampler<decimation_factor_stg3> downsampler_3{};

    Iir3 filter_4_1{Iir3::MakeIir3(iir_stg4_num_1, iir_stg4_den_1)};
    Iir3 filter_4_2{Iir3::MakeIir3(iir_stg4_num_2, iir_stg4_den_2)};
    Iir3 filter_4_3{Iir3::MakeIir3(iir_stg4_num_3, iir_stg4_den_3)};
    Iir3 filter_4_4{Iir3::MakeIir3(iir_stg4_num_4, iir_stg4_den_4)};
    Iir3 filter_4_5{Iir3::MakeIir3(iir_stg4_num_5, iir_stg4_den_5)};
    Gain gain_4_1{iir_stg4_gain_1};
    Gain gain_4_2{iir_stg4_gain_1};
    DownSampler<decimation_factor_stg4> downsampler_4{};

    Chain decimator{std::vector<SignalProcessor*>{
        &filter_1, &downsampler_1, &filter_2, &downsampler_2, &filter_3_1,
        &filter_3_2, &filter_3_3, &gain_3, &downsampler_3, &filter_4_1,
        &filter_4_2, &filter_4_3, &filter_4_4, &filter_4_5, &gain_4_1,
        &gain_4_2, &downsampler_4}};
#endif
}
