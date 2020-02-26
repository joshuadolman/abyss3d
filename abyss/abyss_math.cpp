#ifndef ABYSS_MATH_CPP
#define ABYSS_MATH_CPP

#include "abyss_math.h"

namespace maths {

inline s32
Clamp(s32 value, s32 min, s32 max) {
    if(value >= max) {
        return max;
    }
    else if(value <= min) {
        return min;
    } else {
        return value;
    }
}

inline s32
SafeSubtract(s32 a, s32 b) {
    if(a < S32MIN + b) {
        return S32MIN;
    }
    else if(a > S32MAX - b) {
        return S32MAX;
    } else {
        return a - b;
    }
}

}




#endif //ABYSS_MATH_CPP