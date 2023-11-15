#pragma once

#include "GameEffect.hpp"
#include "Geometry.hpp"
#include "Color.hpp"
#include "ColorChannel.hpp"
#include "easing.h"

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

    static std::unique_ptr<ColorChange> copyColorChange(std::shared_ptr<ColorChannel> channelToCopy, HSVAColor inheritedDelta, float duration, float blending) {
        std::unique_ptr<ColorChange> result = std::make_unique<ColorChange>();

        result->m_copyColor = true;
        result->m_channelToCopy = channelToCopy;
        result->m_inheritedDelta = inheritedDelta;
        result->m_duration = duration;
        result->m_toValue.m_blending = blending;

        result->m_cacheAfter = std::numeric_limits<float>::infinity(); // Never cache, we do it manually

        return result;
    };

    bool m_copyColor = false;
    std::shared_ptr<ColorChannel> m_channelToCopy;
    HSVAColor m_inheritedDelta;
    RGBAColor m_lastCalculated;
    RGBAColor m_cachedCopy;
    float m_position;

    ColorChannelValue valueFor(float x) {
        if (m_copyColor) {
            RGBAColor toCopy = m_channelToCopy->valueFor(m_position + x);

            if (x >= m_duration && m_lastCalculated == toCopy) return {m_cachedCopy, m_toValue.m_blending};
            m_lastCalculated = m_channelToCopy->m_currColor;
            
            auto color = toCopy.toHSVA();
            color.h += m_inheritedDelta.h;
            color.h = fmod(color.h, 360.0f);
            
            if (m_inheritedDelta.addS) color.s += m_inheritedDelta.s;
            else color.s *= m_inheritedDelta.s;

            color.s = std::clamp(color.s, 0.0f, 1.0f);

            if (m_inheritedDelta.addV) color.v += m_inheritedDelta.v;
            else color.v *= m_inheritedDelta.v;

            color.v = std::clamp(color.v, 0.0f, 1.0f);

            m_cachedCopy = color.toRGBA();
            if (m_duration == 0.0f) return {m_cachedCopy, m_toValue.m_blending};
            return {RGBAColor::lerp(m_fromValue, m_cachedCopy, std::min(x / m_duration, 1.0f)), m_toValue.m_blending};
        }

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
    
    easingFunction func;
    float rate;

    CompoundTypeChanger<PositionValue>(PositionValue toValue, float duration, easingFunction func, float rate) : PositionChangeBase(toValue, duration) {
        this->func = func;
        this->rate = rate;
    };

    PositionValue finalValue(PositionValue startValue) {
        return {startValue.val + m_toValue.val};
    };

    PositionValue valueFor(float x) {
        PositionValue result = {m_fromValue.val + (m_toValue.val * func(std::min(x / m_duration, 1.0f), rate))};

        for (auto& [time, changes] : m_changes) {
            for (auto& change : changes) {
                result = {result.val + change->m_toValue.val * change->func(std::min((x - time) / change->m_duration, 1.0f), change->rate)};
            }
        }

        return result;
    };
};