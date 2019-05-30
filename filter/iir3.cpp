#include <filter/iir3.h>

Iir3::Iir3(float _a0, float _a1, float _a2, float _b0, float _b1, float _b2)
    : a0(_a0), a1(_a1), a2(_a2), b0(_b0), b1(_b1), b2(_b2) {}

Iir3 Iir3::MakeIir3(const float as[3], const float bs[3]) {
    return Iir3{as[0], as[1], as[2], bs[0], bs[1], bs[2]};
}

tl::optional<float> Iir3::execute(tl::optional<float> sample) {
    if (!sample) return tl::nullopt;

    // Perform rotation
    x[2] = x[1];
    x[1] = x[0];
    y[2] = y[1];
    y[1] = y[0];

    x[0] = sample.value();
    // TODO(dingbenjamin): Potential to optimise out this division
    y[0] = (b0 * x[0] + b1 * x[1] + b2 * x[2]) - (a1 * y[1] + a2 * y[2]) / a0;
    return y[0];
}
