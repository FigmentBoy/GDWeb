#pragma once

#include "GameEffect.hpp"
#include "Color.hpp"
#include <iostream>

class SpeedChange : public TypeChanger<float> {
public:
    using TypeChanger::TypeChanger;

    SpeedChange(float toValue) {
        m_toValue = toValue;
        m_fromValue = 0.0f;
        m_duration = std::numeric_limits<float>::infinity(); // Never cache
    };

    float valueFor(float x) {
        return m_fromValue + x / m_toValue;
    };
};

struct ColorChannelValue : public RGBAColor {
    bool m_blending;
};

class ColorChange : public TypeChanger<ColorChannelValue> {
public:
    using TypeChanger::TypeChanger;

    ColorChannelValue valueFor(float x) {
        if (m_duration == 0.0f) return m_toValue;
        return {RGBAColor::lerp(m_fromValue, m_toValue, std::min(x / m_duration, 1.0f)), m_toValue.m_blending};
    };
};

class AlphaChange : public TypeChanger<float> {
public:
    using TypeChanger::TypeChanger;

    float valueFor(float x) {
        if (m_duration == 0.0f) return m_toValue;
        return std::lerp(m_fromValue, m_toValue, std::min(x / m_duration, 1.0f));
    };
};