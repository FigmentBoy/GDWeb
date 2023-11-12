#pragma once

#include "GameEffect.hpp"
#include "Geometry.hpp"
#include "Color.hpp"
#include <iostream>

struct SpeedValue {
    float val;
};

$CreateTypeChanger(SpeedValue, Speed) {
public:
    using SpeedChangeBase::SpeedChangeBase;

    TypeChanger(SpeedValue toValue) {
        m_toValue = toValue;
        m_fromValue = { 0.0f };
        m_duration = 0.0f;
        m_cacheAfter = std::numeric_limits<float>::infinity(); // Never cache
    };

    SpeedValue valueFor(float x) {
        return { m_fromValue.val + x / m_toValue.val };
    };
};

struct ColorChannelValue : public RGBAColor {
    bool m_blending;
};

$CreateTypeChanger(ColorChannelValue, Color) {
public:
    using ColorChangeBase::ColorChangeBase;

    ColorChannelValue valueFor(float x) {
        if (m_duration == 0.0f) return m_toValue;
        return {RGBAColor::lerp(m_fromValue, m_toValue, std::min(x / m_duration, 1.0f)), m_toValue.m_blending};
    };
};

struct AlphaValue {
    float val;
};

$CreateTypeChanger(AlphaValue, Alpha) {
public:
    using AlphaChangeBase::AlphaChangeBase;

    AlphaValue valueFor(float x) {
        if (m_duration == 0.0f) return m_toValue;
        return {std::lerp(m_fromValue.val, m_toValue.val, std::min(x / m_duration, 1.0f))};
    };
};

struct PositionValue {
    Point val;
};

$CreateCompoundTypeChanger(PositionValue, Position) {
public:
    using PositionChangeBase::PositionChangeBase;
    
    PositionValue finalValue(PositionValue startValue) {
        return {startValue.val + m_toValue.val};
    };

    PositionValue valueFor(float x) {
        PositionValue result = {m_fromValue.val + (m_toValue.val * std::min(x / m_duration, 1.0f))};

        for (auto& [time, change] : m_changes) {
            result = {result.val + change.m_toValue.val * std::min((x - time) / change.m_duration, 1.0f)};
        }

        return result;
    };
};