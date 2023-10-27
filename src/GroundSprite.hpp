#pragma once

#include "Node.hpp"
#include "Sprite.hpp"
#include "RepeatForeverSprite.hpp"
#include "LevelLayer.hpp"

class GroundSprite : public Node {
public:
    std::vector<std::shared_ptr<RepeatForeverSprite>> m_sprites;
    std::shared_ptr<Sprite> m_shineSprite;

    GroundSprite(int index, LevelLayer* layer);
    void update(float delta);
};