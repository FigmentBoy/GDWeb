#include "Group.hpp"
#include "LevelLayer.hpp"
#include <iostream>

void GroupGroup::updateAlpha() {
    float alpha = 1.f;

    for (auto& group : m_groups) {
        if (!group->m_toggled) {
            alpha = 0.f;
            break;
        }
        
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
    if (!dirty) {
        return;
    }

    dirty = false;
    Point position = {0, 0};
    std::vector<std::pair<float, float>> lockTimes;

    for (auto& group : m_groups) {
        position += group->m_position;
        lockTimes.insert(lockTimes.end(), group->m_lockTimes.begin(), group->m_lockTimes.end());
    }

    lockTimes = merge(lockTimes);

    for (auto& [start, end] : lockTimes) {
        position.x += m_layer->xForTime(end) - m_layer->xForTime(start);
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

    auto oldVal = m_position;
    auto oldLockTimes = m_lockTimes;

    auto newVal = m_positionChanges->valueFor(time);

    m_position = newVal.val;
    m_lockTimes = newVal.lockTimes;

    if (oldVal != m_position || oldLockTimes != m_lockTimes) {
        for (auto& groupGroup : m_groupGroups) {
            groupGroup->dirty = true;
        }
    }
}

void Group::updateToggleChanges(float time) {
    if (!m_toggleChanges) {
        return;
    }

    bool oldVal = m_toggled;
    m_toggled = m_toggleChanges->valueFor(time).val;
    if (oldVal != m_toggled) {
        for (auto& groupGroup : m_groupGroups) {
            groupGroup->updateAlpha();
        }
    }
}

void Group::addStop(float time) {
    for (auto& child : m_childTriggers) {
        child->m_stopAfter = std::min(child->m_stopAfter, time - child->m_position);
    }
}