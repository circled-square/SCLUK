#ifndef SCLUK_MATH_HPP
#define SCLUK_MATH_HPP

#include <cmath>
#include <bit>
#include <numbers>

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
            b * consteval_pow(b, exp-1);
    }

    //defined in .cpp for f32, f64 and f128
    template <typename float_t>
    float_t hann_window(float_t p) {
        constexpr float_t pi = std::numbers::pi_v<float_t>;
        return float_t(.5) * (float_t(1.) + std::cos(pi * (p + float_t(.5))));
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
