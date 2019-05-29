#ifndef SIGNAL_PROCESSOR_H_
#define SIGNAL_PROCESSOR_H_

#include <external/optional.hpp>

class SignalProcessor {
   public:
    virtual tl::optional<float> execute(tl::optional<float> sample) = 0;
};

#endif  // SIGNAL_PROCESSOR_H_
