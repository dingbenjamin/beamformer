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

// Converts readings from the 8 channels into a byte array for UART transmission
void SliceToBuffer(uint8_t buffer[2 * 8], uint16_t i1, uint16_t q1, uint16_t i2,
                   uint16_t q2, uint16_t i3, uint16_t q3, uint16_t i4,
                   uint16_t q4) {
    memcpy(buffer, &i1, 2);
    memcpy(buffer + 2, &q1, 2);
    memcpy(buffer + 4, &i2, 2);
    memcpy(buffer + 6, &q2, 2);
    memcpy(buffer + 8, &i3, 2);
    memcpy(buffer + 10, &q3, 2);
    memcpy(buffer + 12, &i4, 2);
    memcpy(buffer + 14, &q4, 2);
}

/* Timer_A Continuous Mode Configuration Parameter */
const Timer_A_UpModeConfig upModeConfig =
{
        TIMER_A_CLOCKSOURCE_ACLK,            // ACLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,       // ACLK/1 = 32Khz
        1,
        TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, // Disable CCR0
        TIMER_A_DO_CLEAR                     // Clear Counter
};

/* Timer_A Compare Configuration Parameter */
const Timer_A_CompareModeConfig compareConfig =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_SET_RESET,               // Toggle output but
        1                                           // 16000 Period
};

Uart uart;

uint16_t results_buffer[8];      // 8 channels
uint8_t transmit_buffer[8 * 2];  // 8 channels, 2 bytes from each
volatile bool processing = false;

int main(void) {
    // Halting WDT
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_enableSleepOnIsrExit();

    // Configuring GPIO as an output
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);
    ROM_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);

    // Setting up clocks
//    CS_setExternalClockSourceFrequency(32000, 48000000);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1); // 48MHz

    // Initializing ADC (MCLK/1/1)
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
                         0);

    // Configuring GPIOs for Analog In
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P6, GPIO_PIN0 | GPIO_PIN1, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P4,
        GPIO_PIN0 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN7,
        GPIO_TERTIARY_MODULE_FUNCTION);

    // Configuring ADC Memory (ADC_MEM0 - ADC_MEM7 (A0 - A7)  with no repeat)
    // with internal 3.3v reference
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

    // Configuring Timer_A in continuous mode and sourced from MCLK
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);

    // Configuring Timer_A0 in CCR1 to trigger at 16000 (0.5s)
    MAP_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);

    // Configuring the sample trigger to be sourced from Timer_A0  and setting
    // it to automatic iteration after it is triggered*/
    MAP_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);

    // Enabling the interrupt when a conversion on channel 7 (end of sequence)
    //  is complete and enabling conversions
    MAP_ADC14_enableInterrupt(ADC_INT7);
    MAP_ADC14_enableConversion();

    // Enabling Interrupts
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    // Starting the Timer
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    // Going to sleep
    while (1) {
    }
}

extern "C" {

void ADC14_IRQHandler(void) {
    uint64_t status;
    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);
    MAP_Interrupt_disableMaster();
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN7);

    if (status & ADC_INT7) {
        MAP_ADC14_getMultiSequenceResult(results_buffer);
        uart.Write(reinterpret_cast<uint8_t*>(results_buffer), 8 * 2);
    }

    MAP_Interrupt_enableMaster();
}

}

#endif
