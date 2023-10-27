#include "Group.hpp"
#include <iostream>

void Group::updateAlphaTriggers(float time) {
    if (!m_alphaTriggers) {
        return;
    }

    float state = m_alphaTriggers->valueFor(time);  
    if (*m_currAlpha != state) {
        *m_currAlpha = state;
    }
}