#include <filter/decimator.h>

tl::optional<float> Decimator::execute(tl::optional<float> sample) {
    return chain.execute(sample);
}
