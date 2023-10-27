#pragma once

#include "Node.hpp"
#include "RepeatForeverSprite.hpp"
#include "LevelLayer.hpp"

class BackgroundSprite : public Node {
public:
    std::shared_ptr<RepeatForeverSprite> m_sprite;

    BackgroundSprite(int index, LevelLayer* layer);
    void update(float delta);
};