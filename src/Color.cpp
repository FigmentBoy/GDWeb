#include "Color.hpp"

#include <algorithm>

RGBAColor HSVAColor::toRGBA() {
    RGBAColor out;

    double hh = h;
    if (hh >= 360) hh = 0;
    hh /= 60;
    long i = (long)hh;
    double ff = hh - i;
    
    double p = v * (1.0 - s);
    double q = v * (1.0 - (s * ff));
    double t = v * (1.0 - (s * (1.0 - ff)));

    switch(i) {
        case (0):
            out.r = v;
            out.g = t;
            out.b = p;
            break;
        case (1):
            out.r = q;
            out.g = v;
            out.b = p;
            break;
        case (2):
            out.r = p;
            out.g = v;
            out.b = t;
            break;
        case (3):
            out.r = p;
            out.g = q;
            out.b = v;
            break;
        case (4):
            out.r = t;
            out.g = p;
            out.b = v;
            break;
        case (5):
        default:
            out.r = v;
            out.g = p;
            out.b = q;
            break;
    }
    
    out.a = a;
    return out;
}

RGBAColor RGBAColor::lerp(RGBAColor from, RGBAColor to, float t) {
    RGBAColor out;

    out.r = from.r + (to.r - from.r) * t;
    out.g = from.g + (to.g - from.g) * t;
    out.b = from.b + (to.b - from.b) * t;
    out.a = from.a + (to.a - from.a) * t;

    return out;
}

HSVAColor RGBAColor::toHSVA() {
    HSVAColor out;

    double min = std::min(std::min(r, g), b);
    double max = std::max(std::max(r, g), b);

    out.v = max;
    double delta = max - min;

    if (delta < 0.00001) {
        out.s = 0;
        out.h = 0;
        return out;
    }

    if (max > 0.0) {
        out.s = (delta / max);
    } else {
        out.s = 0.0;
        out.h = NAN;
        return out;
    }

    if (r >= max) {
        out.h = (g - b) / delta;
    } else {
        if (g >= max) {
            out.h = 2.0 + (b - r) / delta;
        } else {
            out.h = 4.0 + (r - g) / delta;
        }
    }

    out.h *= 60.0;

    if (out.h < 0.0) {
        out.h += 360.0;
    }

    out.a = a;
    return out;
}