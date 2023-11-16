#include "ChannelTriggers.hpp"
#include "ColorChannel.hpp"

std::unique_ptr<ColorChange> ColorChange::copyColorChange(std::shared_ptr<ColorChannel> channelToCopy, HSVAColor inheritedDelta, float duration, float blending) {
    std::unique_ptr<ColorChange> result = std::make_unique<ColorChange>();

    result->m_copyColor = true;
    result->m_channelToCopy = channelToCopy;
    result->m_inheritedDelta = inheritedDelta;
    result->m_duration = duration;
    result->m_toValue.m_blending = blending;

    result->m_cacheAfter = std::numeric_limits<float>::infinity(); // Never cache, we do it manually

    return result;
};

ColorChannelValue ColorChange::valueFor(float x) {
    if (m_copyColor) {
        RGBAColor baseColor = m_channelToCopy->valueFor(m_position + x);

        if (m_lastBase != baseColor) {
            m_lastBase = baseColor;
            m_toValue = {baseColor.shift(m_inheritedDelta), m_toValue.m_blending};
        }
    }

    if (m_duration == 0.0f || x > m_duration) return m_toValue;
    return {RGBAColor::lerp(m_fromValue, m_toValue, std::min(x / m_duration, 1.0f)), m_toValue.m_blending};
};

PulseValue PulseChange::finalValue(PulseValue startValue) {
    return startValue;
}

PulseValue PulseChange::getContribution(PulseValue in, float x) {
    PulseValue out = in;
    float percent = 1.0f;

    if (x < m_fadeIn) {
        if (m_fadeIn == 0.0f) percent = 0.0f;
        else percent = x / m_fadeIn;
    } else if (x > m_fadeIn + m_hold) {
        if (m_fadeOut == 0.0f) percent = 1.0f;
        else percent = 1.0f - ((x - m_fadeIn - m_hold) / m_fadeOut);
    }

    percent = std::clamp(percent, 0.0f, 1.0f);

    if (m_pulseType == PulseType::HSV) {
        if (m_channelToCopy == nullptr) {
            auto baseColor = m_targetChannel->valueForWithoutPulses(m_position + x);

            if (m_prevCopyColor != baseColor) {
                m_toRGB = baseColor.shift(m_toHSV);
                m_prevCopyColor = baseColor;
            }
        } else {
            auto baseColor = m_channelToCopy->valueFor(m_position + x);

            if (m_prevCopyColor != baseColor) {
                m_toRGB = baseColor.shift(m_toHSV);
                m_prevCopyColor = baseColor;
            }
        }
    }

    out = RGBAColor::lerp(out, m_toRGB, percent);

    return out;
}

PulseValue PulseChange::valueFor(float x) {
    RGBAColor base = m_targetChannel->valueForWithoutPulses(m_position + x);
    PulseValue result = base;

    for (auto it = m_changes.begin(); it != m_changes.end(); it++) {
        auto& [time, changes] = *it;
        for (auto& change : changes) {
            if (change->m_exclusive) {
                result = base;
                result = change->getContribution(result, x - time);
            } else {
                result = change->getContribution(result, x - time);
            }
        }
    }

    result = getContribution(result, x);

    return result;
}