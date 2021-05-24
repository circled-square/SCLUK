#ifndef SCLUK_RANGE_HELPER_HPP
#define SCLUK_RANGE_HELPER_HPP

#include <type_traits>
#include <concepts>
#include <cstdint>

namespace scluk {
    inline namespace ranges {
        template<std::integral T = std::int64_t>
        struct range {
            using int_t = T;
            const int_t lower_bound, upper_bound;
            range(int_t lower, int_t upper) : lower_bound(lower), upper_bound(upper) { }
            range(int_t upper) : range(0, upper) { }

            struct iterator_t {
                int_t el;
                bool       operator!=(int_t o) { return el != o; }
                iterator_t operator++()        { return iterator_t { el++ }; }
                int_t&     operator* ()        { return el;}
            };
            iterator_t begin() { return iterator_t { lower_bound }; }
            int_t end() { return upper_bound; }
        };
        template<typename T> range(T)   -> range<T>;//template deduction guides
        template<typename T> range(T,T) -> range<T>;
        
        inline range<std::size_t> index(const auto& c) { return range<std::size_t>(c.size()); }

        struct range_shorthand_helper {
            const std::int64_t lower_bound;
            range<std::int64_t> operator>(std::int64_t upper_bound) {
                return range<std::int64_t>(lower_bound, upper_bound);
            }
            range_shorthand_helper operator-() { return range_shorthand_helper { -lower_bound }; }
        };
        inline range_shorthand_helper operator""_(unsigned long long lower_bound) {
            return range_shorthand_helper { std::int64_t(lower_bound) };
        }
    }
}

#endif
