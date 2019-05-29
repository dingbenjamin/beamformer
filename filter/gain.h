#ifndef FILTER_GAIN_H_
#define FILTER_GAIN_H_

#include <filter/signal_processor.h>
#include <external/optional.hpp>

class Gain : public SignalProcessor {
    float gain;

   public:
    Gain(float _gain);
    tl::optional<float> execute(tl::optional<float> sample);
};

#endif  // FILTER_GAIN_H_
