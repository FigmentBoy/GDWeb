#pragma once

#include "GameEffect.hpp"
#include "Geometry.hpp"
#include "easing.h"

struct AlphaValue {
    float val;
};

$CreateTypeChanger(AlphaValue, Alpha) {
public:
    using AlphaChangeBase::AlphaChangeBase;

    AlphaValue valueFor(float x);
};

struct ToggleValue {
    bool val;
};

$CreateTypeChanger(ToggleValue, Toggle) {
public:
    using ToggleChangeBase::ToggleChangeBase;

    ToggleValue valueFor(float x);
};

struct PositionValue {
    Point val;
    std::vector<std::pair<float, float>> lockTimes = {};
};

class LevelLayer;

$CreateCompoundTypeChanger(PositionValue, Position) {
public:
    using PositionChangeBase::PositionChangeBase;
    
    easingFunction func = [](float t, float r) { return t; };
    float rate;
    bool lockX = false;
    LevelLayer* layer;

    CompoundTypeChanger<PositionValue>(PositionValue toValue, float duration, easingFunction func, float rate, bool lockX, LevelLayer* layer) : PositionChangeBase(toValue, duration), func(func), rate(rate), lockX(lockX), layer(layer) {};

    PositionValue finalValue(PositionValue startValue);
    PositionValue getContribution(float x);
    PositionValue valueFor(float x);
};

