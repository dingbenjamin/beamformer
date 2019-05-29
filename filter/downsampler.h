#ifndef FILTER_DOWNSAMPLER_H_
#define FILTER_DOWNSAMPLER_H_

#include <filter/signal_processor.h>
#include <stdint.h>
#include <external/optional.hpp>

template <uint16_t Factor>
class DownSampler : public SignalProcessor {
    uint16_t counter{0};

   public:
    tl::optional<float> execute(tl::optional<float> sample) {
        if (!sample) {
            return tl::nullopt;
        } else if (counter == Factor) {
            counter = 0;
            return sample;
        } else {
            counter++;
            return tl::nullopt;
        }
    }
};

#endif  // FILTER_DOWNSAMPLER_H_
