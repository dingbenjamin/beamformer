#include <filter/chain.h>
#include <utility>

Chain::Chain(std::vector<SignalProcessor*>&& _blocks) : blocks(std::move(_blocks)) {}

tl::optional<float> Chain::execute(tl::optional<float> sample) {
    tl::optional<float> result{sample};
    for (auto block : blocks) {
        result = block->execute(result);
    }
    return result;
}
