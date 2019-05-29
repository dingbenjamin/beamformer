#include <filter/gain.h>

Gain::Gain(float _gain) : gain(_gain) {}

tl::optional<float> Gain::execute(tl::optional<float> sample) {
    if (!sample) {
        return tl::nullopt;
    } else {
        return gain * sample.value();
    }
}
