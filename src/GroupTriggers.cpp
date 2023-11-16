#include "GroupTriggers.hpp"
#include "LevelLayer.hpp"

AlphaValue AlphaChange::valueFor(float x) {
    if (m_duration == 0.0f) return m_toValue;
    return {std::lerp(m_fromValue.val, m_toValue.val, std::min(x / m_duration, 1.0f))};
}

ToggleValue ToggleChange::valueFor(float x) {
    return m_toValue;
}

PositionValue PositionChange::finalValue(PositionValue startValue) {
    return {startValue.val + m_toValue.val};
}

Point PositionChange::getContribution(float x) {
    Point val = m_toValue.val * func(std::min(x / m_duration, 1.0f), rate);
    if (lockX) {
        val.x += layer->xForTime(m_positionTime + std::min(x, m_duration)) - m_positionX;
    }
    return val;
}

PositionValue PositionChange::valueFor(float x)  {
    PositionValue result = {m_fromValue.val + getContribution(x)};

    for (auto& [time, changes] : m_changes) {
        for (auto& change : changes) {
            result.val += change->getContribution(x - time);
        }
    }

    return result;
}