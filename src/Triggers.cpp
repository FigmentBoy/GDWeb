#include "Triggers.hpp"
#include "LevelLayer.hpp"

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