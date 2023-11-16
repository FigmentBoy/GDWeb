#pragma once

#include "GameEffect.hpp"

#include "Color.hpp"
#include <memory>

class ColorChannel;

struct ColorChannelValue : public RGBAColor {
    bool m_blending;
};

$CreateTypeChanger(ColorChannelValue, Color) {
public:
    using ColorChangeBase::ColorChangeBase;

    bool m_copyColor = false;
    std::shared_ptr<ColorChannel> m_channelToCopy;
    HSVAColor m_inheritedDelta;

    RGBAColor m_lastBase = {-1, -1, -1};
    float m_position;

    static std::unique_ptr<ColorChange> copyColorChange(std::shared_ptr<ColorChannel> channelToCopy, HSVAColor inheritedDelta, float duration, float blending);
    ColorChannelValue valueFor(float x);
};

struct PulseValue : public RGBAColor {
    PulseValue() {};
    PulseValue(RGBAColor color) : RGBAColor(color) {};
};

enum class PulseType {
    RGB = 0,
    HSV = 1
};

$CreateCompoundTypeChanger(PulseValue, Pulse) {
public:
    using PulseChangeBase::PulseChangeBase;

    CompoundTypeChanger<PulseValue>(bool exclusive, PulseType pulseType, RGBAColor toRGB, std::shared_ptr<ColorChannel> targetChannel, std::shared_ptr<ColorChannel> channelToCopy, HSVAColor toHSV, float fadeIn, float hold, float fadeOut) 
        : m_exclusive(exclusive), m_pulseType(pulseType), m_toRGB(toRGB), m_targetChannel(targetChannel), m_channelToCopy(channelToCopy), m_toHSV(toHSV), m_fadeIn(fadeIn), m_hold(hold), m_fadeOut(fadeOut) {
            m_duration = fadeIn + hold + fadeOut;
            m_cacheAfter = m_duration;
    };

    bool m_exclusive;

    std::shared_ptr<ColorChannel> m_targetChannel;
    std::shared_ptr<ColorChannel> m_channelToCopy;
    HSVAColor m_toHSV;
    RGBAColor m_toRGB;
    PulseType m_pulseType;
    
    float m_position;

    float m_fadeIn;
    float m_hold;
    float m_fadeOut;

    RGBAColor m_prevCopyColor = {-1, -1, -1};

    PulseValue finalValue(PulseValue startValue);
    PulseValue getContribution(PulseValue in, float x);
    PulseValue valueFor(float x);
};
