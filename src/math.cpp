#include "../include/scluk/math.hpp"

namespace scluk::math {
    //quake reverse square root algorithm bit magic
    f32 quake_rsqrt(f32 number) {
        const f32 x2 = number * .5f;
        u32 i = std::bit_cast<u32>(number);
        i = 0x5f3759df - (i >> 1);
        number = std::bit_cast<f32>(i);
        number  *= 1.5f - (x2 * number*number);
        return number;
    }
}
