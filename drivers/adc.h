#ifndef DRIVERS_ADC_H_
#define DRIVERS_ADC_H_

#include <stdint.h>

// Does not support more than one instantiation
class Adc {
   public:
    static constexpr float kBadReading = -1;

    // TODO(dingbenjamin): See if we can achieve 5V ref
    static constexpr float kRefVoltage = 3.3;

    struct ArraySlice {
        float i1{kBadReading};
        float q1{kBadReading};
        float i2{kBadReading};
        float q2{kBadReading};
        float i3{kBadReading};
        float q3{kBadReading};
        float i4{kBadReading};
        float q4{kBadReading};
    };

    Adc();
    ArraySlice Sample();

   private:
    float RawReadingToFloat(uint16_t raw);
};

#endif  // DRIVERS_ADC_H_
