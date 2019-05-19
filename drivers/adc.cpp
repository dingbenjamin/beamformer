#include <drivers/adc.h>
#include <string.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// TODO(dingbenjamin): Find a nicer way of doing this
bool converted = false;
uint16_t results_buffer[8];

Adc::Adc() {
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_enableSleepOnIsrExit();

    /* Zero-filling buffer */
    memset(results_buffer, 0x00, 8 * sizeof(uint16_t));

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
     * with internal 2.5v reference */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM7, false);

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
    /// Pin 6.0
    MAP_ADC14_configureConversionMemory(
        ADC_MEM7, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A15, false);

    /* Enabling the interrupt when a conversion on channel 7 (end of sequence)
     *  is complete and enabling conversions */
    MAP_ADC14_enableInterrupt(ADC_INT7);

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence
     * convert. */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}

Adc::ArraySlice Adc::Sample() {
    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();

    while (!converted) {
    }
    converted = false;

    return Adc::ArraySlice{
            RawReadingToFloat(results_buffer[7]),
            RawReadingToFloat(results_buffer[3]),
            RawReadingToFloat(results_buffer[7]),
            RawReadingToFloat(results_buffer[6]),
            RawReadingToFloat(results_buffer[5]),
            RawReadingToFloat(results_buffer[2]),
            RawReadingToFloat(results_buffer[1]),
            RawReadingToFloat(results_buffer[0])
    };
}

float Adc::RawReadingToFloat(uint16_t raw) {
    return raw * kRefVoltage / (static_cast<uint16_t>(2) << 14);
}

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM7. This signals the end of conversion and the results array is
 * grabbed and placed in results_buffer */
void ADC14_IRQHandler() {
    uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (status & ADC_INT7) {
        MAP_ADC14_getMultiSequenceResult(results_buffer);
    }

    converted = true;
}
