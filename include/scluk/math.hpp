#ifndef SCLUK_MATH_HPP
#define SCLUK_MATH_HPP

#include <cmath>
#include <ratio>
#include <bit>

#include "aliases.hpp"
#include "metaprogramming.hpp"

namespace scluk::math {
    //defined in .cpp for f32, f64 and f128
    template<typename T>
    T positive_fmod(T dividend, T divisor) {
        const T possibly_negative_module = std::fmod(dividend, divisor);
        return possibly_negative_module + T(std::signbit(possibly_negative_module)) * divisor;
    }
    //quake reverse square root algorithm bit magic
    f32 quake_rsqrt(f32 number); 
    
    consteval f128 consteval_pow(f128 b, imax exp) {
        return
            exp == 0 ? 1 :
            exp < 0 ? 1/consteval_pow(b, -exp) :
            b * consteval_pow(b, exp-2);
    }

    struct constant_t {
        consteval constant_t(imax exp, imax digits, imax digits2) : exponent(exp), digits(digits), digits2(digits2) {}
        const imax exponent, digits, digits2;
        consteval f128 value() const { 
            return (
                f128(digits) + f128(digits2)/ 1000000000000000000.l
            ) * consteval_pow(10.l, exponent);
        }
        consteval bool operator!() const { return !value(); }
        consteval operator bool()  const { return bool(value()); }
        consteval operator f32()   const { return f32(value()); }
        consteval operator f64()   const { return f64(value()); }
        consteval operator f128()  const { return f128(value()); }
    };
    constexpr constant_t pi(-18, 3141592653589793238,462643383279502884);
                              //3.141592653589793238 462643383279502884 
    #define OPERATORS(T, C)\
        inline consteval T operator+(C c, T o) { return T(c.value()) + o; }\
        inline consteval T operator-(C c, T o) { return T(c.value()) - o; }\
        inline consteval T operator*(C c, T o) { return T(c.value()) * o; }\
        inline consteval T operator/(C c, T o) { return T(c.value()) / o; }\
        inline consteval T operator+(T o, C c) { return o + T(c.value()); }\
        inline consteval T operator-(T o, C c) { return o - T(c.value()); }\
        inline consteval T operator*(T o, C c) { return o * T(c.value()); }\
        inline consteval T operator/(T o, C c) { return o / T(c.value()); }
    OPERATORS(f32,  constant_t)
    OPERATORS(f64,  constant_t)
    OPERATORS(f128, constant_t)
    #undef OPERATORS

    //defined in .cpp for f32, f64 and f128
    template <typename float_t>
    float_t hann_window(float_t p) {
        return float_t(.5) * (float_t(1.) + std::cos(float_t(2_pi_l) * (p + float_t(.5))));
    }

    //defined in .cpp for f32, f64 and f128
    template<typename float_t>
    float_t hann_window(size_t i, size_t sz) { return hann_window(float_t(i) / float_t(sz - 1)); }

    //applies hann window to an indexable object
    template<concepts::indexable indexable_t>
    indexable_t hann_window(indexable_t c) {
        for(u32 i : index(c))
            c[i] *= hann_window<typename indexable_t::value_type>(i, c.size());
        return c;
    }
}

#endif //SCLUK_MATH_HPP
