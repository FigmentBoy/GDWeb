#include "GroundSprite.hpp"

#include "Director.hpp"
#include "SpriteFrameCache.hpp"

GroundSprite::GroundSprite(int index, LevelLayer* layer) {
    if (index == 0) index = 1;
    if (index < 8) {
        std::string name = (std::string) "groundSquare_0" + std::to_string(index) + "_001.png";
        std::shared_ptr<RepeatForeverSprite> sprite = std::make_shared<RepeatForeverSprite>(SpriteFrameCache::get()->getSpriteFrameByName(name), 128.f);
        sprite->m_position = {0, -128};
        sprite->m_zOrder = 0.0f;
        addChild(sprite);

        m_sprites.push_back(sprite);

        sprite->m_colorChannel = 1001;
    } else {
        std::string baseName = (std::string) "groundSquare_" + (index < 10 ? "0" : "") + std::to_string(index) + "_001.png";
        std::shared_ptr<SpriteFrame> baseFrame = SpriteFrameCache::get()->getSpriteFrameByName(baseName);
        std::shared_ptr<RepeatForeverSprite> baseSprite = std::make_shared<RepeatForeverSprite>(baseFrame, 128.f);
        baseSprite->m_position = {0, -128};
        baseSprite->m_zOrder = 0.0f;
        addChild(baseSprite);

        std::string topName = (std::string) "groundSquare_" + (index < 10 ? "0" : "") + std::to_string(index) + "_2_001.png";
        std::shared_ptr<SpriteFrame> topFrame = SpriteFrameCache::get()->getSpriteFrameByName(topName);
        std::shared_ptr<RepeatForeverSprite> topSprite = std::make_shared<RepeatForeverSprite>(topFrame, topFrame->m_size.height / 4.f);
        topSprite->m_position = {0.f, -topFrame->m_size.height / 4.f};
        topSprite->m_zOrder = 1.0f;
        addChild(topSprite);

        m_sprites.push_back(baseSprite);
        m_sprites.push_back(topSprite);

        baseSprite->m_colorChannel = 1001;
        topSprite->m_colorChannel = 1001;
    }

    std::shared_ptr<SpriteFrame> shineFrame = SpriteFrameCache::get()->getSpriteFrameByName("floorLine_01_001.png");
    m_shineSprite = std::make_shared<Sprite>(shineFrame);

    auto camera = Director::get()->m_camera;
    m_shineSprite->m_position = {camera->m_position.x + (camera->m_viewSize.x * camera->m_viewScale.x * 0.5f), 1-shineFrame->m_size.height};
    m_shineSprite->m_zOrder = 2.0f;
    addChild(m_shineSprite);

    m_dirtyMatrix = true;
    for (auto sprite : m_sprites) {
        sprite->m_dirty = true;
    }
    m_shineSprite->m_dirty = true;
}

void GroundSprite::update(float delta) {
    auto camera = Director::get()->m_camera;
    float offset = fmod(camera->m_position.x + 512, 128);

    float oldX = m_position.x;
    m_position.x = camera->m_position.x - offset;
    
    if (oldX != m_position.x) {
        m_dirtyMatrix = true;

        for (auto sprite : m_sprites) {
            sprite->m_dirty = true;
        }
    }

    float oldShineX = m_shineSprite->m_position.x;
    m_shineSprite->m_position.x = offset + (camera->m_viewSize.x * camera->m_viewScale.x * 0.5f);

    if (oldShineX != m_shineSprite->m_position.x) {
        m_shineSprite->m_dirtyMatrix = true;
        m_shineSprite->m_dirty = true;
    }

    Node::update(delta);
}