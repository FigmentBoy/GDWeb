#pragma once

#include <map>
#include <iostream>
#include "utils.hpp"


#define $CreateTypeChanger(type, name)                                          \
    using name##ChangeBase = BaseTypeChanger<type>;                             \
    using name##Change = TypeChanger<type>;                                     \
    template <>                                                                 \
    class TypeChanger<type> : public BaseTypeChanger<type>                      \
    
#define $CreateCompoundTypeChanger(type, name)                                  \
    using name##ChangeBase = BaseCompoundTypeChanger<type>;                     \
    using name##Change = CompoundTypeChanger<type>;                             \
    template <>                                                                 \
    class CompoundTypeChanger<type> : public BaseCompoundTypeChanger<type>      \

template <typename T>
class TypeChanger;

template <typename T>
class BaseTypeChanger {
public:
    using Type = T;

    Type m_toValue;
    Type m_fromValue;
    float m_duration;
    float m_cacheAfter;

    BaseTypeChanger() {};
    BaseTypeChanger(Type toValue) {
        m_toValue = toValue;
        m_fromValue = toValue;
        m_duration = -std::numeric_limits<float>::infinity();
        m_cacheAfter = m_duration;
    }
    BaseTypeChanger(Type toValue, float duration) {
        m_toValue = toValue;
        m_fromValue = toValue;
        m_duration = duration;
        m_cacheAfter = m_duration;
    }
    BaseTypeChanger(Type toValue, Type fromValue, float duration) : m_toValue(toValue), m_fromValue(fromValue), m_duration(duration), m_cacheAfter(duration) {};

    /**
     * @brief Returns the value of this changer at a given time
     * 
     * @param x The offset from the start of the changer
     * @return Type The value at the given offset
     */
    virtual Type valueFor(float x) = 0;
};

template <typename T>
class CompoundTypeChanger;

template <typename T>
class BaseCompoundTypeChanger : public BaseTypeChanger<T> {
public:
    using Type = T;
    using BaseTypeChanger<T>::BaseTypeChanger;

    std::map<float, CompoundTypeChanger<Type>> m_changes;

    /**
     * @brief Returns the final value of only this changer, not including any sub-changers, given a starting value
     * 
     * @param startValue The starting value
     * @return Type The final value
     */
    virtual Type finalValue(Type startValue) = 0;

    /**
     * @brief Returns the value of this changer at a given time
     * 
     * @param x The offset from the start of the changer
     * @return Type The value at the given offset
     */
    virtual Type valueFor(float x) = 0;
};

template <typename T>
class BaseGameEffect {
public:
    using Changer = T;
    using Type = Changer::Type;

    std::map<float, Changer> m_changes;
    Type m_startingValue;
    
    Type m_cachedValue;
    float m_cachedValueMin = std::numeric_limits<float>::infinity();
    float m_cachedValueMax = -std::numeric_limits<float>::infinity();

    BaseGameEffect(Type startingValue) : m_startingValue(startingValue) {}

    virtual Type valueFor(float x) {
        if (x > m_cachedValueMin && x < m_cachedValueMax) {
            return m_cachedValue;
        }

        typename std::map<float, Changer>::iterator lowerBound = m_changes.lower_bound(x);
        if (lowerBound == m_changes.begin()) {
            Changer changer = Changer(m_startingValue);

            if (x < changer.m_cacheAfter) return changer.valueFor(x);
        
            m_cachedValue = changer.valueFor(x);
            m_cachedValueMin = changer.m_cacheAfter;
            m_cachedValueMax = lowerBound->first;

            return m_cachedValue;
        }

        auto iter = std::prev(lowerBound);

        if (x < iter->first + iter->second.m_cacheAfter) return iter->second.valueFor(x - iter->first);

        m_cachedValue = iter->second.valueFor(x - iter->first);

        m_cachedValueMin = iter->first + iter->second.m_cacheAfter;
        if (lowerBound == m_changes.end()) m_cachedValueMax = std::numeric_limits<float>::infinity();
        else m_cachedValueMax = lowerBound->first;

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

template <typename T>
class GameEffect;

template <typename T>
class GameEffect<TypeChanger<T>> : public BaseGameEffect<TypeChanger<T>> {
public:
    using Changer = class TypeChanger<T>;
    using Type = T;

    using BaseGameEffect<TypeChanger<T>>::BaseGameEffect;

    virtual void setup() {
        float lastPosition = 0.0f;
        Changer lastChanger = Changer(this->m_startingValue);

        for (auto& [position, change] : this->m_changes) {
            change.m_fromValue = lastChanger.valueFor(position - lastPosition);
            lastPosition = position;
            lastChanger = change;
        }
    }
};

template <typename T>
class GameEffect<CompoundTypeChanger<T>> : public BaseGameEffect<CompoundTypeChanger<T>> {
public:
    using Changer = class CompoundTypeChanger<T>;
    using Type = T;

    using BaseGameEffect<CompoundTypeChanger<T>>::BaseGameEffect;

    virtual void setup() {
        std::map<float, Changer> activeChangers;
        Type lastCommittedValue = this->m_startingValue;

        for (auto& [position, change] : this->m_changes) {
            for (auto& [activePosition, activeChanger] : activeChangers) {
                if (activePosition + activeChanger.m_duration < position) {
                    lastCommittedValue = activeChanger.finalValue(lastCommittedValue);
                    activeChangers.erase(activePosition);
                } else {
                    change.m_changes[activePosition - position] = activeChanger;
                    change.m_cacheAfter = std::max(change.m_cacheAfter, activeChanger.m_duration - (activePosition - position));
                }
            }
            
            change.m_fromValue = lastCommittedValue;
            activeChangers[position] = change;
        }
    }
};