#include "Group.hpp"
#include <iostream>

void GroupGroup::updateAlpha() {
    float alpha = 1.f;
    for (auto& group : m_groups) {
        alpha *= group->m_currAlpha;
    }

    GLfloat data[4] = {
        alpha, 0, 0, 0
    };

    m_groupTexture->setSubData(
        data,
        {1, 1}, {0, m_index}, GL_FLOAT
    );
}

void GroupGroup::updatePosition() {
    Point position = {0, 0};
    for (auto& group : m_groups) {
        position += group->m_position;
    }

    GLfloat data[4] = {
        position.x, position.y, 0, 0
    };

    m_groupTexture->setSubData(
        data,
        {1, 1}, {1, m_index}, GL_FLOAT
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

    auto old = m_position;
    m_position = m_positionChanges->valueFor(time).val;
    if (old != m_position) {
        for (auto& groupGroup : m_groupGroups) {
            groupGroup->updatePosition();
        }
    }
}