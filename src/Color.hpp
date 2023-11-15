#pragma once

class RGBAColor;
class HSVAColor;

class HSVAColor {
public:
    float h;
    float s;
    float v;
    float a;

    bool addS = false;
    bool addV = false;

    HSVAColor() : h(0), s(0), v(0), a(1) {}
    HSVAColor(float _h, float _s, float _v, float _a) : h(_h), s(_s), v(_v), a(_a) {}
    HSVAColor(float _h, float _s, float _v) : h(_h), s(_s), v(_v), a(1) {}

    RGBAColor toRGBA();
};

class RGBAColor {
public:
    float r;
    float g;
    float b;
    float a;

    RGBAColor() : r(0), g(0), b(0), a(1) {}
    RGBAColor(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
    RGBAColor(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1) {}

    static RGBAColor lerp(RGBAColor from, RGBAColor to, float t);

    bool operator==(const RGBAColor& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    bool operator!=(const RGBAColor& other) const {
        return !(*this == other);
    }

    HSVAColor toHSVA();
};