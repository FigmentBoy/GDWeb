#pragma once

#include "GameObject.hpp"
#include "Triggers.hpp"

class Group {
public:
    int m_index;
    
    Point m_position;
    float m_rotation;
    std::shared_ptr<float> m_currAlpha;

    std::unique_ptr<GameEffect<AlphaChange>> m_alphaTriggers;
    

    Group() : m_position(0, 0), m_rotation(0), m_currAlpha(std::make_shared<float>(1.f)) {};

    void updateAlphaTriggers(float time);

    // TODO: add color trigger stuff
};