#include "BackgroundSprite.hpp"

#include "Director.hpp"
#include "SpriteFrameCache.hpp"

BackgroundSprite::BackgroundSprite(int index, LevelLayer* layer) {
    auto camera = Director::get()->m_camera;
    if (index == 0) index = 1;

    m_position = {-1000.f, -1000.f};

    std::string name = (std::string) "game_bg_" + (index < 10 ? "0" : "") + std::to_string(index) + "_001.png";
    m_sprite = std::make_shared<RepeatForeverSprite>(SpriteFrameCache::get()->getSpriteFrameByName(name), camera->m_viewSize.y * camera->m_viewScale.y);
    m_sprite->m_position = {0, 0};
    addChild(m_sprite);

    m_sprite->m_colorChannel = layer->m_colorChannels[1000];
}

void BackgroundSprite::update(float delta) {
    auto camera = Director::get()->m_camera;

    if (m_position.x != camera->m_position.x || m_position.y != camera->m_position.y) {
        m_position = {camera->m_position.x, camera->m_position.y};
        m_sprite->m_lockedHeight = camera->m_viewSize.y * camera->m_viewScale.y;

        m_dirtyMatrix = true;
        m_sprite->m_dirty = true;
    }

    Node::update(delta);
}