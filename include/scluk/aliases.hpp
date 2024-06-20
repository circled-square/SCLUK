#ifndef SCLUK_ALIASES_HPP
#define SCLUK_ALIASES_HPP

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <climits>

namespace scluk {
    using i8    = std::int8_t;
    using i16   = std::int16_t;
    using i32   = std::int32_t;
    using i64   = std::int64_t;
    using u8    = std::uint8_t;
    using u16   = std::uint16_t;
    using u32   = std::uint32_t;
    using u64   = std::uint64_t;

    using imax  = std::intmax_t;
    using umax  = std::uintmax_t;
    using uint  = unsigned;
    using uchar = unsigned char;
    using schar = signed char;
    using std::size_t;

    using f32   = std::conditional_t<sizeof(float) * CHAR_BIT == 32, float, void>;
    using f64   = std::conditional_t<sizeof(double) * CHAR_BIT == 64, double, void>;
    using f128  = std::conditional_t<sizeof(long double) * CHAR_BIT == 128, long double, void>;
    using fmax  = long double;

    //define integer literal operators _u8, _u16 ... _i8, _i16 ...
    #define TMP_MACRO(T) constexpr T operator""_##T(unsigned long long n) { return T(n); }
        TMP_MACRO(u8)TMP_MACRO(u16)TMP_MACRO(u32)TMP_MACRO(u64)TMP_MACRO(umax)
        TMP_MACRO(i8)TMP_MACRO(i16)TMP_MACRO(i32)TMP_MACRO(i64)TMP_MACRO(imax)
    #undef TMP_MACRO
}

#endif
