#ifndef SCLUK_RANGE_HELPER_HPP
#define SCLUK_RANGE_HELPER_HPP

#include "aliases.hpp"

namespace scluk {
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
}

#endif
