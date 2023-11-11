#pragma once

#include "GameObject.hpp"
#include "Triggers.hpp"

class Group {
public:
    int m_index;
    
    Point m_position;
    float m_rotation;
    float m_currAlpha;

    std::unique_ptr<GameEffect<AlphaChange>> m_alphaTriggers;
    std::unique_ptr<GameEffect<PositionChange>> m_positionChanges;

    std::vector<std::shared_ptr<GameObject>> m_objects;

    Group() : m_position(0, 0), m_rotation(0), m_currAlpha(1.f) {};

    void updateAlphaTriggers(float time);
    void updatePositionChanges(float time);
};