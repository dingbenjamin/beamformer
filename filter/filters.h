#ifndef FILTER_FILTERS_H_
#define FILTER_FILTERS_H_

#include <stdint.h>
#include <algorithm>
#include <external/optional.hpp>
#include <vector>

class SignalProcessor {
   public:
    virtual tl::optional<float> execute(tl::optional<float> sample) = 0;
};

// TODO(dingbenjamin): Would be super super cool to make this tempalte variadic,
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

    Iir3(float _a0, float _a1, float _a2, float _b0, float _b1, float _b2)
        : a0(_a0), a1(_a1), a2(_a2), b0(_b0), b1(_b1), b2(_b2) {}

   public:
    static Iir3 MakeIir3(const float as[3], const float bs[3]) {
        return Iir3(as[0], as[1], as[2], bs[0], bs[1], bs[2]);
    }

    tl::optional<float> execute(tl::optional<float> sample) {
        if (!sample) return tl::nullopt;

        std::rotate(&x[1], &x[1], &x[2]);
        std::rotate(&y[0], &y[1], &y[2]);
        x[0] = sample.value();
        // TODO(dingbenjamin): Potential to optimise out this division
        y[0] =
            (a0 * x[0] + a1 * x[1] + a2 * x[2] - b1 * (y[1] + b2 * y[2])) / b0;
        return y[0];
    }
};

template <uint16_t Factor>
class DownSampler : public SignalProcessor {
    uint16_t counter{0};

   public:
    DownSampler(){};

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

class Gain : public SignalProcessor {
    float gain;

   public:
    Gain(float _gain) : gain(_gain) {}

    tl::optional<float> execute(tl::optional<float> sample) {
        if (!sample) {
            return tl::nullopt;
        } else {
            return gain * sample.value();
        }
    }
};

class Chain : public SignalProcessor {
    std::vector<SignalProcessor*> blocks;

   public:
    Chain(std::vector<SignalProcessor*> _blocks) : blocks(_blocks) {}

    tl::optional<float> execute(tl::optional<float> sample) {
        tl::optional<float> result{sample};
        for (auto block : blocks) {
            result = block->execute(result);
        }
        return result;
    }
};

#endif  // FILTER_FILTERS_H_
