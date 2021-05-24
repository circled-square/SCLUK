#ifndef SCLUK_SLIDING_MEAN_HPP
#define SCLUK_SLIDING_MEAN_HPP

#include "sliding_queue.hpp"

namespace scluk::math {
    template <typename T, std::size_t sz>
    class sliding_mean {
        T sum, mean;
        sliding_queue<T, sz> q;
    public:
        inline sliding_mean() : sum(0), mean(0) {}
        inline T push(T new_val) {
            T old_val = q.push(new_val);
            sum = sum + new_val - old_val;
            return mean = sum / sz;
        }

        inline T get() { return mean; }
    };
}


#endif //SCLUK_SLIDING_MEAN_HPP
