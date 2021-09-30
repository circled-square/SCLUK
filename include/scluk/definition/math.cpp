#include "../math.hpp"

namespace scluk::math {
    template f32 positive_fmod<f32>(f32, f32);
    template f64 positive_fmod<f64>(f64, f64);
    template f128 positive_fmod<f128>(f128, f128);
    
    template f32 hann_window<f32>(f32);
    template f64 hann_window<f64>(f64);
    template f128 hann_window<f128>(f128);
    
    template f32 hann_window<f32>(size_t, size_t);
    template f64 hann_window<f64>(size_t, size_t);
    template f128 hann_window<f128>(size_t, size_t);

    //quake reverse square root algorithm bit magic
    //f32 quake_rsqrt(f32 number) {
    //    const f32 x2 = number * .5f;
    //    u32 i = std::bit_cast<u32>(number);
    //    i = 0x5f3759df - (i >> 1);
    //    number = std::bit_cast<f32>(i);
    //    number  *= 1.5f - (x2 * number*number);
    //    return number;
    //}

     consteval f128 consteval_pow(f128 b, imax exp) {
        return
            exp == 0 ? 1 :
            exp < 0 ? 1/consteval_pow(b, -exp) :
            b * consteval_pow(b, exp-2);
    }           
}
