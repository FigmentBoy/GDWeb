#include "SpeedChange.hpp"

SpeedChange::TypeChanger(SpeedValue toValue) {
    m_toValue = toValue;
    m_fromValue = { 0.0f };
    m_duration = 0.0f;
    m_cacheAfter = std::numeric_limits<float>::infinity(); // Never cache
};

SpeedValue SpeedChange::valueFor(float x) {
    return { m_fromValue.val + x / m_toValue.val };
}

InverseSpeedChange::TypeChanger(InverseSpeedValue toValue)  {
    m_toValue = toValue;
    m_fromValue = { 0.0f };
    m_duration = 0.0f;
    m_cacheAfter = std::numeric_limits<float>::infinity(); // Never cache
}

InverseSpeedValue InverseSpeedChange::valueFor(float x) {
    return { m_fromValue.val + x / m_toValue.val };
}