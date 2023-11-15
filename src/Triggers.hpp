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

struct InverseSpeedValue {
    float val;
};

$CreateTypeChanger(InverseSpeedValue, InverseSpeed) {
public:
    using InverseSpeedChangeBase::InverseSpeedChangeBase;

    TypeChanger(InverseSpeedValue toValue) {
        m_toValue = toValue;
        m_fromValue = { 0.0f };
        m_duration = 0.0f;
        m_cacheAfter = std::numeric_limits<float>::infinity(); // Never cache
    };

    InverseSpeedValue valueFor(float x) {
        return { m_fromValue.val + x / m_toValue.val };
    };
};

struct ColorChannelValue : public RGBAColor {
    bool m_blending;
};

class LevelLayer;

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

struct ToggleValue {
    bool val;
};

$CreateTypeChanger(ToggleValue, Toggle) {
public:
    using ToggleChangeBase::ToggleChangeBase;

    ToggleValue valueFor(float x) {
        return m_toValue;
    };
};

struct PositionValue {
    Point val;
};

$CreateCompoundTypeChanger(PositionValue, Position) {
public:
    using PositionChangeBase::PositionChangeBase;
    
    float m_positionX;
    float m_positionTime;
    easingFunction func = [](float t, float r) { return t; };
    float rate;
    bool lockX = false;
    LevelLayer* layer;

    CompoundTypeChanger<PositionValue>(PositionValue toValue, float duration, easingFunction func, float rate, float m_positionX, bool lockX, LevelLayer* layer) : PositionChangeBase(toValue, duration), func(func), rate(rate), m_positionX(m_positionX), lockX(lockX), layer(layer) {};

    PositionValue finalValue(PositionValue startValue);
    Point getContribution(float x);
    PositionValue valueFor(float x);
};

// struct BasePulseValue {
//     float percent = 1.0f;

//     BasePulseValue() {};
//     BasePulseValue(float percent) : percent(percent) {};

//     virtual RGBAColor apply(RGBAColor in) = 0;
// };

struct RGBPulseValue : public RGBAColor {
    float percent;

    RGBPulseValue(RGBAColor color, float percent) : RGBAColor(color), percent(percent) {};

    RGBAColor apply(RGBAColor in) {
        return RGBAColor::lerp(in, *this, percent);
    }
};

struct PulseValue {
    std::vector<std::shared_ptr<RGBPulseValue>> pulses;
};

enum class PulseType {
    RGB = 0,
    HSV = 1
};

$CreateCompoundTypeChanger(PulseValue, Pulse) {
public:
    using PulseChangeBase::PulseChangeBase;

    CompoundTypeChanger<PulseValue>(bool exclusive, PulseType hsvColorType, RGBAColor toRGB, std::shared_ptr<ColorChannel> channelToCopy, HSVAColor toHSV, float fadeIn, float hold, float fadeOut)  {
        this->m_exclusive = exclusive;
        this->hsvColorType = hsvColorType;
        this->toRGB = toRGB;
        this->channelToCopy = channelToCopy;
        this->toHSV = toHSV;
        this->fadeIn = fadeIn;
        this->hold = hold;
        this->fadeOut = fadeOut;
        this->m_duration = this->fadeIn + this->hold + this->fadeOut;
        this->m_cacheAfter = this->fadeIn + this->hold + this->fadeOut;
    };

    bool m_exclusive = false;

    std::shared_ptr<ColorChannel> channelToCopy;
    HSVAColor toHSV;
    RGBAColor toRGB;
    PulseType hsvColorType;

    float fadeIn;
    float hold;
    float fadeOut;

    RGBAColor prevCopyColor = {-1, -1, -1};
    RGBAColor copiedDelta = {-1, -1, -1};

    PulseValue finalValue(PulseValue startValue) {
        return startValue;
    }

    PulseValue getContribution(PulseValue in, float x) {
        PulseValue out = in;
        float percent = 1.0f;

        if (x < fadeIn) {
            percent = x / fadeIn;
        } else if (x > fadeIn + hold) {
            percent = 1.0f - ((x - fadeIn - hold) / fadeOut);
        }

        percent = std::clamp(percent, 0.0f, 1.0f);

        if (percent > 0.0f) {
            switch (hsvColorType) {
                case PulseType::RGB: {
                    out.pulses.push_back(std::make_shared<RGBPulseValue>(toRGB, percent));
                    break;
                }
                case PulseType::HSV: {
                    auto copyColor = channelToCopy->m_currColor;
                    if (copyColor != prevCopyColor) {
                        auto color = channelToCopy->m_currColor.toHSVA();

                        color.h += toHSV.h * percent;
                        color.h = fmod(color.h + 360.0f, 360.0f);

                        if (toHSV.addS) color.s += toHSV.s * percent;
                        else color.s *= std::lerp(1, toHSV.s, percent);
                        color.s = std::clamp(color.s, 0.0f, 1.0f);

                        if (toHSV.addV) color.v += toHSV.v * percent;
                        else color.v *= std::lerp(1, toHSV.v, percent);
                        color.v = std::clamp(color.v, 0.0f, 1.0f);

                        copiedDelta = color.toRGBA();
                    }
                    out.pulses.push_back(std::make_shared<RGBPulseValue>(copiedDelta, percent));
                    break;
                }
            }
        }

        return out;
    }

    PulseValue valueFor(float x) {
        PulseValue result;
        if (!m_exclusive) {
            for (auto& [time, changes] : m_changes) {
                for (auto& change : changes) {
                    result = change->getContribution(result, x - time);
                }
            }
        }   
        result = getContribution(result, x);

        return result;
    }

    static RGBAColor apply(RGBAColor in, PulseValue delta) {
        RGBAColor out = in;
        for (auto& pulse : delta.pulses) {
            out = pulse->apply(out);
        }
        return out;
    }
};
