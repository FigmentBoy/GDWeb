#include "Group.hpp"
#include <iostream>

void Group::updateAlphaTriggers(float time) {
    if (!m_alphaTriggers) {
        return;
    }

    float val = m_alphaTriggers->valueFor(time).val;
    if (m_currAlpha != val) {
        m_currAlpha = val;
        for (auto& object : m_objects) {
            object->m_dirtyAlpha = true;
        }
    }
}

void Group::updatePositionChanges(float time) {
    if (!m_positionChanges) {
        return;
    }

    Point val = m_positionChanges->valueFor(time).val;
    if (m_position != val) {
        m_position = val;
        for (auto& object : m_objects) {
            object->m_dirtyPosition = true;
        }
    }
}