#pragma once

#include "Sprite.hpp"
class RepeatForeverSprite : public Sprite {
public:
    RepeatForeverSprite(std::shared_ptr<SpriteFrame> frame, float height) : Sprite(frame), m_lockedHeight(height) {m_anchorPoint = {0,0};};
    float m_lockedHeight;

    void updateVertices();
};