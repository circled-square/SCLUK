#ifndef SCLUK_TUPLE_HPP
#define SCLUK_TUPLE_HPP

#include <tuple>

namespace scluk {
    template<typename T> using omogenous_pair = std::pair<T,T>;

    template<typename tuple_t> inline consteval
    std::size_t tuple_sz(tuple_t) { return std::tuple_size<tuple_t>::value; }

    template<typename tuple_t> inline consteval
    std::size_t tuple_sz() { return std::tuple_size<tuple_t>::value; }
}

#endif
