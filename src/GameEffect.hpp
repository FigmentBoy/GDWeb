#pragma once

#include <map>
#include <iostream>

template <typename T>
class TypeChanger {
public:
    using Type = T;

    Type m_toValue;
    Type m_fromValue;
    float m_duration;

    TypeChanger() {};
    TypeChanger(Type toValue) {
        m_toValue = toValue;
        m_fromValue = toValue;
        m_duration = -std::numeric_limits<float>::infinity();
    }
    TypeChanger(Type toValue, float duration) {
        m_toValue = toValue;
        m_fromValue = toValue;
        m_duration = duration;
    }
    TypeChanger(Type toValue, Type fromValue, float duration) : m_toValue(toValue), m_fromValue(fromValue), m_duration(duration) {};

    virtual Type valueFor(float x) = 0;
};

template <typename Changer>
class GameEffect {
public:
    using Type = typename Changer::Type;

    std::map<float, Changer> m_changes;
    Type m_startingValue;
    
    Type m_cachedValue;
    float m_cachedValueMin = 0.f;
    float m_cachedValueMax = 0.f;

    GameEffect(Type startingValue) : m_startingValue(startingValue) {}

    virtual void setup() {
        float lastPosition = 0.0f;
        Changer lastChanger = Changer(m_startingValue);

        for (auto& [position, change] : m_changes) {
            change.m_fromValue = lastChanger.valueFor(position - lastPosition);
            lastPosition = position;
            lastChanger = change;
        }
    }

    virtual Type valueFor(float x) {
        if (x > m_cachedValueMin && x < m_cachedValueMax) {
            return m_cachedValue;
        }

        typename std::map<float, Changer>::iterator lowerBound = m_changes.lower_bound(x);
        if (lowerBound == m_changes.begin()) {
            Changer changer = Changer(m_startingValue);
            m_cachedValue = changer.valueFor(x);
            m_cachedValueMin = changer.m_duration;
            m_cachedValueMax = lowerBound->first;

            return m_cachedValue;
        }

        auto iter = std::prev(lowerBound);

        m_cachedValue = iter->second.valueFor(x - iter->first);

        m_cachedValueMin = iter->first + iter->second.m_duration;
        if (lowerBound == m_changes.end()) m_cachedValueMax = std::numeric_limits<float>::infinity();
        else m_cachedValueMax = lowerBound->first;

        if (m_cachedValueMin > x) m_cachedValueMin = std::numeric_limits<float>::infinity(); // If we skip to after the duration, still update

        return m_cachedValue;
    };

    virtual Changer mostRecent(float x) {
        typename std::map<float, Changer>::iterator lowerBound = m_changes.lower_bound(x);
        if (lowerBound == m_changes.begin()) {
            return Changer(m_startingValue);
        }
        auto iter = --lowerBound;
        return iter->second;
    };
};