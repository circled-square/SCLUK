#ifndef SCLUK_MATH_HPP
#define SCLUK_MATH_HPP

#include <cmath>
#include <ratio>
#include <bit>

#include "aliases.hpp"
#include "metaprogramming.hpp"

namespace scluk::math {
    template<typename T>
    T positive_module(T dividend, T divisor) {
        const T possibly_negative_module = std::fmod(dividend, divisor);
        return possibly_negative_module + T(std::signbit(possibly_negative_module)) * divisor;
    }
    /*//to use this I must wait until the implementation of std::bit_cast into gcc
    inline constexpr f32 fast_rsqrt(f32 number) noexcept {
        const f32 x2 = number * .5f;
        u32 i = std::bit_cast<u32>(number);
        i = 0x5f3759df - (i >> 1);
        number = std::bit_cast<f32>(i);
        number  *= 1.5f - (x2 * number*number );
        return number;
    }*/
    inline f32 quake_rsqrt(f32 n) {	
        const f32 x2 = n * 0.5f;

        union {
            f32 f;
            u32 i;
        } conv = { .f = n };
        conv.i = 0x5f3759df - (conv.i >> 1);
        conv.f *= 1.5f - (x2 * conv.f*conv.f);
        return conv.f;
    }
    constexpr f128 constexpr_pow(f128 b, imax exp) {
        return 
            exp == 0 ? 1 :
            exp < 0 ? 1/constexpr_pow(b, -exp) :
            b * constexpr_pow(b, exp-1);
    }

    template<imax exponent, imax digits, imax digits2 = 0>
    class constant_t {
    public:
        static constexpr f128 value = (
            f128(digits) 
            + f128(digits2)/ 1000000000000000000.l
        ) * constexpr_pow(10.l, exponent);
        inline constexpr bool operator!() const { return !value; }
        inline constexpr operator bool()  const { return bool(value); }
        inline constexpr operator f32()   const { return f32(value); }
        inline constexpr operator f64()   const { return f64(value); }
        inline constexpr operator f128()  const { return f128(value); }
    };
    using pi_constant_t = constant_t<-18, 3141592653589793238,462643383279502884>;
                                        //3.141592653589793238462643383279502884 
    #define OPERATORS(T, C)\
        inline constexpr T operator+(C, T o) { return T(C::value) + o; }\
        inline constexpr T operator-(C, T o) { return T(C::value) - o; }\
        inline constexpr T operator*(C, T o) { return T(C::value) * o; }\
        inline constexpr T operator/(C, T o) { return T(C::value) / o; }\
        inline constexpr T operator+(T o, C) { return o + T(C::value); }\
        inline constexpr T operator-(T o, C) { return o - T(C::value); }\
        inline constexpr T operator*(T o, C) { return o * T(C::value); }\
        inline constexpr T operator/(T o, C) { return o / T(C::value); }
    OPERATORS(f32, pi_constant_t)
    OPERATORS(f64, pi_constant_t)
    OPERATORS(f128, pi_constant_t)

    constexpr pi_constant_t pi;

    inline namespace literals {
        constexpr f128 operator""_pi_l(long double n)        { return pi * n; }
        constexpr f128 operator""_pi_l(unsigned long long n) { return pi * f128(n); }

        constexpr f64 operator""_pi(long double n)           { return pi * f64(n); }
        constexpr f64 operator""_pi(unsigned long long n)    { return pi * f64(n); }

        constexpr f32 operator""_pi_f(long double n)         { return pi * f32(n); }
        constexpr f32 operator""_pi_f(unsigned long long n)  { return pi * f32(n); }
    }
    template <std::floating_point float_t>
    float_t hann_window(float_t p) {
        return float_t(.5) * (float_t(1.) + std::cos(float_t(2_pi_l) * (p + float_t(.5))));
    }

    template<std::floating_point float_t>
    float_t hann_window(size_t i, size_t sz) { return hann_window(float_t(i) / (sz - 1)); }

    template<concepts::indexable indexable_t>
    indexable_t hann_window(indexable_t c) {
        for(u32 i : index(c))
            c[i] *= hann_window<typename std::remove_cvref<decltype(c[0])>::type>(i, c.size());
        return c;
    }
}

namespace scluk { namespace math_literals = math::literals; }

#endif //SCLUK_MATH_HPP
