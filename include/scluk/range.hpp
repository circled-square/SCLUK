#ifndef SCLUK_RANGE_HELPER_HPP
#define SCLUK_RANGE_HELPER_HPP

#include <type_traits>
#include <concepts>
#include <cstdint>
#include "metaprogramming.hpp"
#include "aliases.hpp"

namespace scluk {
    inline namespace ranges {
        struct range {
            const i64 lower_bound, upper_bound;

            range(i64 upper);
            range(size_t upper);
            range(i64 lower, i64 upper);
            range(size_t lower, size_t upper);

            struct iterator {
                i64 el;
                bool     operator!=(i64 o);
                iterator operator++();
                i64&     operator* ();
            };
            iterator begin();
            i64 end();
        };
        
        inline range index(const auto& c) { return range(c.size()); }

        struct range_shorthand_helper {
            const i64 lower_bound;
            range operator>(i64 upper_bound);
            range_shorthand_helper operator-();
        };
        range_shorthand_helper operator""_(unsigned long long lower_bound);
    }
}

#endif
