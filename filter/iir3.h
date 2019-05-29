#ifndef FILTER_IIR3_H_
#define FILTER_IIR3_H_

#include <filter/signal_processor.h>
#include <algorithm>
#include <external/optional.hpp>

// TODO(dingbenjamin): Would be super super cool to make this template variadic,
// but no need for this project
class Iir3 : public SignalProcessor {
   private:
    float x[3] = {0.00, 0.00, 0.00};
    float y[3] = {0.00, 0.00, 0.00};

    // TODO(dingbenjamin): These aren't compile time constants for now because
    // it's a pain to template with floats
    const float a0;
    const float a1;
    const float a2;
    const float b0;
    const float b1;
    const float b2;

    Iir3(float _a0, float _a1, float _a2, float _b0, float _b1, float _b2);

   public:
    static Iir3 MakeIir3(const float as[3], const float bs[3]);
    tl::optional<float> execute(tl::optional<float> sample);
};

#endif  // FILTER_IIR3_H_
