#include "SectionBatcher.hpp"

void SectionBatcher::addBatchSprite(std::shared_ptr<Sprite> sprite) {
    if (sprite->m_currentBatcher == this) return;
    sprite->removeFromBatcher();

    if (std::find(m_textures.begin(), m_textures.end(), sprite->m_spriteFrame->m_texture) == m_textures.end()) {
        m_textures.push_back(sprite->m_spriteFrame->m_texture);
    }

    int section = sprite->getBoundingBox().left() / SECTION_WIDTH;

    m_sections[section][sprite->m_batchZLayer][sprite->m_zOrder].push_back(sprite);
    
    recurseAdd(sprite);
}

void SectionBatcher::removeBatchSprite(std::shared_ptr<Sprite> sprite) {
    if (sprite->m_currentBatcher != this) return;
    
    sprite->m_currentBatcher = nullptr;

    for (auto& [position, zOrderMap] : m_sections) {
        for (auto& [zOrder, zLayerMap] : zOrderMap) {
            for (auto& [zLayer, sprites] : zLayerMap) {
                sprites.erase(std::remove(sprites.begin(), sprites.end(), sprite), sprites.end());
            }
        }
    }

    for (auto child : sprite->m_children) {
        if (std::shared_ptr<Sprite> sprite = std::dynamic_pointer_cast<Sprite>(child)) {
            removeBatchSprite(sprite);
        }
    }
}

void SectionBatcher::draw() {
    Rect cameraRect = Director::get()->m_camera->getViewRect();
    float minX = cameraRect.left() - SECTION_WIDTH;
    float maxX = cameraRect.right() + SECTION_WIDTH;

    m_sprites.clear();
    
    auto first = m_sections.lower_bound(minX / SECTION_WIDTH);
    auto last = m_sections.upper_bound(maxX / SECTION_WIDTH);

    std::vector<int> indicies;
    
    for (auto it = first; it != last; it++) {
        auto zOrderMap = it->second;
        for (auto& [zOrder, zLayerMap] : zOrderMap) {
            for (auto& [zLayer, sprites] : zLayerMap) {
                m_sprites.insert(m_sprites.end(), sprites.begin(), sprites.end());
            }
        }
    }

    Batcher::draw();
}