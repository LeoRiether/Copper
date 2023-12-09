#pragma once

inline float lerp(float from, float to, float p) {
    return from + p * (to - from);
}

inline float qerp_in(float from, float to, float p) {
    return lerp(from, to, p * p);
}

inline float qerp_out(float from, float to, float p) {
    return lerp(from, to, 1 - (1 - p) * (1 - p));
}
