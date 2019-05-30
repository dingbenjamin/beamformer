#ifndef FILTER_CHAIN_H_
#define FILTER_CHAIN_H_

#include <filter/signal_processor.h>
#include <external/optional.hpp>
#include <vector>

class Chain : public SignalProcessor {
    std::vector<SignalProcessor*> blocks;

   public:
    Chain(std::vector<SignalProcessor*>&& _blocks);
    tl::optional<float> execute(tl::optional<float> sample);
};

#endif  // FILTER_CHAIN_H_
