#include <commands/command.h>
#include <drivers/dac.h>
#include <drivers/pa.h>
#include <drivers/uart.h>
#include <filter/decimator.h>
#include <nanopb/pb_decode.h>
#include <stdio.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <transmitter.h>
#include <cassert>
#include <functional>
#include <vector>
#include "msp.h"

#ifdef TRANSMIT

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
}

#endif

#ifdef RECEIVE

/* Timer_A Continuous Mode Configuration Parameter */
const Timer_A_UpModeConfig upModeConfig = {
    TIMER_A_CLOCKSOURCE_ACLK,       // ACLK Clock Source
    TIMER_A_CLOCKSOURCE_DIVIDER_1,  // ACLK/1 = 32Khz
    16384,
    TIMER_A_TAIE_INTERRUPT_DISABLE,       // Disable Timer ISR
    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,  // Disable CCR0
    TIMER_A_DO_CLEAR                      // Clear Counter
};

/* Timer_A Compare Configuration Parameter */
const Timer_A_CompareModeConfig compareConfig = {
    TIMER_A_CAPTURECOMPARE_REGISTER_1,         // Use CCR1
    TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,  // Disable CCR interrupt
    TIMER_A_OUTPUTMODE_SET_RESET,              // Toggle output but
    16384                                      // 16000 Period
};

struct ArraySlice {
    float i1;
    float q1;
    float i2;
    float q2;
    float i3;
    float q3;
    float i4;
    float q4;
};

float RawReadingToFloat(uint16_t raw) {
    return raw * 3.3 / (static_cast<uint16_t>(1) << 14);
}

Decimator decimator_i1;
Decimator decimator_q1;
Decimator decimator_i2;
Decimator decimator_q2;
Decimator decimator_i3;
Decimator decimator_q3;
Decimator decimator_i4;
Decimator decimator_q4;

uint16_t results_buffer[8];
bool processing = false;

int main(void) {
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_enableSleepOnIsrExit();

    /* Setting up clocks
     * MCLK = MCLK = 3MHz
     * ACLK = REFO = 32Khz */
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Initializing ADC (MCLK/1/1) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
                         0);

    /* Configuring GPIOs for Analog In */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P6, GPIO_PIN0 | GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P4,
        GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7,
        GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM7 (A0 - A7)  with no repeat)
     * with internal 3.3v reference */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM7, true);

    // Pin 4.7
    MAP_ADC14_configureConversionMemory(
        ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A6, false);
    // Pin 4.5
    MAP_ADC14_configureConversionMemory(
        ADC_MEM1, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A8, false);
    // Pin 4.4
    MAP_ADC14_configureConversionMemory(
        ADC_MEM2, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A9, false);
    // Pin 4.3
    MAP_ADC14_configureConversionMemory(
        ADC_MEM3, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A10, false);
    // Pin 4.2
    MAP_ADC14_configureConversionMemory(
        ADC_MEM4, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A11, false);
    // Pin 4.0
    MAP_ADC14_configureConversionMemory(
        ADC_MEM5, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A13, false);
    // Pin 6.1
    MAP_ADC14_configureConversionMemory(
        ADC_MEM6, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A14, false);
    // Pin 6.0
    MAP_ADC14_configureConversionMemory(
        ADC_MEM7, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A15, false);

    /* Configuring Timer_A in continuous mode and sourced from ACLK */
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);

    // TODO(dingbenjamin): Change to 45kHz
    /* Configuring Timer_A0 in CCR1 to trigger at 16000 (0.5s) */
    MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);

    /* Configuring the sample trigger to be sourced from Timer_A0  and setting
     * it to automatic iteration after it is triggered*/
    MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);

    /* Enabling the interrupt when a conversion on channel 7 (end of sequence)
     *  is complete and enabling conversions */
    MAP_ADC14_enableInterrupt(ADC_INT7);
    MAP_ADC14_enableConversion();

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    /* Starting the Timer */
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    /* Going to sleep */
    while (1) {
        MAP_PCM_gotoLPM0();
    }
}

extern "C" {

void ADC14_IRQHandler(void) {
    uint64_t status;

    // TODO(dingbenjamin): Check to see if the processing is failing timing constraints
    processing = true;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (status & ADC_INT7) {
        MAP_ADC14_getMultiSequenceResult(results_buffer);

        ArraySlice results{RawReadingToFloat(results_buffer[7]),   // i1 : 6.0
                           RawReadingToFloat(results_buffer[3]),   // q1 : 4.3
                           RawReadingToFloat(results_buffer[6]),   // i2 : 6.1
                           RawReadingToFloat(results_buffer[5]),   // q2 : 4.0
                           RawReadingToFloat(results_buffer[4]),   // i3 : 4.2
                           RawReadingToFloat(results_buffer[2]),   // q3 : 4.4
                           RawReadingToFloat(results_buffer[1]),   // i4 : 4.5
                           RawReadingToFloat(results_buffer[0])};  // q4 : 4.7

        auto i1 = decimator_i1.execute(results.i1);
        auto q1 = decimator_q1.execute(results.q1);
        auto i2 = decimator_i1.execute(results.i2);
        auto q2 = decimator_q1.execute(results.q2);
        auto i3 = decimator_i1.execute(results.i3);
        auto q3 = decimator_q1.execute(results.q3);
        auto i4 = decimator_i1.execute(results.i4);
        auto q4 = decimator_q1.execute(results.q4);

        // TODO(dingbenjamin): Pipe out the results over UART
    }

    processing = false;
}

}

#endif
