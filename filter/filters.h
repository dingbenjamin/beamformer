#ifndef FILTER_FILTERS_H_
#define FILTER_FILTERS_H_

#include <algorithm>

// TODO(dingbenjamin): Would be super super cool to make this tempalte variadic,
// but no need for this project
class Iir3 {
   private:
    // TODO(dingbenjamin): Zero initialise?
    float x[3];
    float y[3];

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
    static Iir3 makeIir3(const float as[3], const float bs[3]) {
        return Iir3(as[0], as[1], as[2], bs[0], bs[1], bs[2]);
    }

    float filter(float next_sample) {
        std::rotate(&x[1], &x[1], &x[2]);
        std::rotate(&y[0], &y[1], &y[2]);
        x[0] = next_sample;
        // TODO(dingbenjamin): Potential to optimise out this division
        y[0] =
            (a0 * x[0] + a1 * x[1] + a2 * x[2] - b1 * (y[1] + b2 * y[2])) / b0;
        return y[0];
    }
};

#endif  // FILTER_FILTERS_H_
