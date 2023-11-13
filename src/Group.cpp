#include "Group.hpp"
#include <iostream>

void GroupGroup::updateAlpha() {
    float alpha = 1.f;
    for (auto& group : m_groups) {
        alpha *= group->m_currAlpha;
    }

    GLubyte data[4] = {
        static_cast<GLubyte>(alpha * 255), 
        0, 
        0, 
        0,
    };

    m_groupTexture->setSubData(
        data,
        {1, 1}, {0, m_index}, GL_UNSIGNED_BYTE
    );
}

void Group::updateAlphaTriggers(float time) {
    if (!m_alphaTriggers) {
        return;
    }

    float oldVal = m_currAlpha;
    m_currAlpha = m_alphaTriggers->valueFor(time).val;

    if (oldVal != m_currAlpha) {
        for (auto& groupGroup : m_groupGroups) {
            groupGroup->updateAlpha();
        }
    }
}

void Group::updatePositionChanges(float time) {
    if (!m_positionChanges) {
        return;
    }

    // Point val = m_positionChanges->valueFor(time).val;
    // if (m_position != val) {
    //     m_position = val;
    //     for (auto& object : m_objects) {
    //         object->m_dirtyPosition = true;
    //     }
    // }
}