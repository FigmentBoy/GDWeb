#include "SectionBatcher.hpp"

void SectionBatcher::addBatchSprite(std::shared_ptr<Sprite> sprite) {
    if (sprite->m_currentBatcher == this) return;
    sprite->removeFromBatcher();

    if (std::find(m_textures.begin(), m_textures.end(), sprite->m_spriteFrame->m_texture) == m_textures.end()) {
        m_textures.push_back(sprite->m_spriteFrame->m_texture);
    }

    int section = sprite->getBoundingBox().left() / SECTION_WIDTH;
    m_sections[section].push_back(sprite);
    
    m_sorted = false;

    recurseAdd(sprite);
}

void SectionBatcher::removeBatchSprite(std::shared_ptr<Sprite> sprite) {
    if (sprite->m_currentBatcher != this) return;
    
    sprite->m_currentBatcher = nullptr;

    for (auto section : m_sections) {
        section.second.erase(std::remove(section.second.begin(), section.second.end(), sprite), section.second.end());
    }

    m_sprites.erase(std::remove(m_sprites.begin(), m_sprites.end(), sprite), m_sprites.end());

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
    for (auto& section : m_sections) {
        if (section.first * SECTION_WIDTH > minX && section.first * SECTION_WIDTH < maxX) {
            m_sprites.insert(m_sprites.end(), section.second.begin(), section.second.end());
        }
    }

    Batcher::draw();
}