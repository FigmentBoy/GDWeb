#pragma once

#include "GameEffect.hpp"

struct SpeedValue {
    float val;
};

$CreateTypeChanger(SpeedValue, Speed) {
public:
    using SpeedChangeBase::SpeedChangeBase;

    TypeChanger(SpeedValue toValue);
    SpeedValue valueFor(float x);
};

struct InverseSpeedValue {
    float val;
};

$CreateTypeChanger(InverseSpeedValue, InverseSpeed) {
public:
    using InverseSpeedChangeBase::InverseSpeedChangeBase;

    TypeChanger(InverseSpeedValue toValue);
    InverseSpeedValue valueFor(float x);
};