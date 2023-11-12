#pragma once

#include "Batcher.hpp"
#include "Director.hpp" 
#include "Sprite.hpp"

#include <iostream>
#include <map>
#include <vector>

#define SECTION_WIDTH 100

class SectionBatcher : public Batcher {
public:
    std::map<int, std::map<float, std::map<float, std::vector<std::shared_ptr<Sprite>>>>> m_sections; // shush

    SectionBatcher() : Batcher() {};

    void addBatchSprite(std::shared_ptr<Sprite> sprite);
    void removeBatchSprite(std::shared_ptr<Sprite> sprite);

    void draw();
};