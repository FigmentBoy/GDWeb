#include "ChannelTriggers.hpp"
#include "ColorChannel.hpp"

std::unique_ptr<ColorChange> ColorChange::copyColorChange(std::shared_ptr<ColorChannel> channelToCopy, HSVAColor inheritedDelta, float duration, bool blending, bool copyOpacity, float opacity) {
    std::unique_ptr<ColorChange> result = std::make_unique<ColorChange>();

    result->m_copyColor = true;
    result->m_copyOpacity = copyOpacity;
    result->m_toValue.a = opacity;
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
            float alpha = m_copyOpacity ? baseColor.a : m_toValue.a;
            m_toValue = {baseColor.shift(m_inheritedDelta), m_toValue.m_blending};
            m_toValue.a = alpha;
        }
    }

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
        if (m_fadeOut == 0.0f) percent = 0.0f;
        else percent = 1.0f - ((x - m_fadeIn - m_hold) / m_fadeOut);
    }

    percent = std::clamp(percent, 0.0f, 1.0f);

    if (m_pulseType == PulseType::HSV) {
        auto baseColor = m_channelToCopy->valueForWithoutPulses(m_position + x);

        if (m_prevCopyColor != baseColor) {
            m_toRGB = baseColor.shift(m_toHSV);
            m_prevCopyColor = baseColor;
        }
    }

    out = RGBAColor::lerp(out, m_toRGB, percent);

    return out;
}

PulseValue PulseChange::valueFor(float x) {
    RGBAColor base = m_targetChannel->valueForWithoutPulses(m_position + x);

    if (base == m_lastBase && x == m_lastPosition) {
        return m_lastPulse;
    }

    m_lastPosition = x;
    m_lastBase = base;
    m_lastPulse = base;

    m_lastPulse = getContribution(m_lastPulse, x);

    for (auto it = m_changes.rbegin(); it != m_changes.rend(); it++) {
        auto& [time, changes] = *it;
        for (auto& change : changes) {
            if (change->m_exclusive) {
                m_lastPulse = base;
                m_lastPulse = change->getContribution(m_lastPulse, x - time);
            } else {
                m_lastPulse = change->getContribution(m_lastPulse, x - time);
            }
        }
    }

    return m_lastPulse;
}