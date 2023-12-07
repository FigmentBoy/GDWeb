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
    std::vector<std::pair<float, float>> lockTimes = startValue.lockTimes;
    if (lockX) {
        lockTimes.push_back({m_position, m_position + std::min(m_stopAfter, m_duration)});
        lockTimes = merge(lockTimes);
    }
    return {startValue.val + m_toValue.val, lockTimes};
}

PositionValue PositionChange::getContribution(float x) {
    Point val = m_toValue.val * func(x / m_duration, rate);
    std::vector<std::pair<float, float>> lockTimes = {};
    if (lockX) {
        lockTimes.push_back({m_position, m_position + std::min(x, m_stopAfter)});
    }
    return {val, lockTimes};
}

PositionValue PositionChange::valueFor(float x)  {
    PositionValue result = {m_fromValue.val, {}};
    result.lockTimes.insert(result.lockTimes.end(), m_fromValue.lockTimes.begin(), m_fromValue.lockTimes.end());

    PositionValue contribution = getContribution(x);
    result.val += contribution.val;
    result.lockTimes.insert(result.lockTimes.end(), contribution.lockTimes.begin(), contribution.lockTimes.end());

    for (auto& [time, changes] : m_changes) {
        for (auto& change : changes) {
            PositionValue contribution = change->getContribution(x - time);
            result.val += contribution.val;
            result.lockTimes.insert(result.lockTimes.end(), contribution.lockTimes.begin(), contribution.lockTimes.end());
        }
    }

    result.lockTimes = merge(result.lockTimes);

    return result;
}