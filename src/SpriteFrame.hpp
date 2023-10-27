#pragma once

#include <memory>
#include "Texture.hpp"
#include "Geometry.hpp"

class SpriteFrame {
public:
    std::shared_ptr<Texture> m_texture;
    Rect m_texCoords;
    Point m_offset;
    Size m_size;
    bool m_rotated = false;
    
    SpriteFrame() : m_texCoords({0.0f, 0.0f}, {1.0f, 1.0f}), m_offset({0.0f, 0.0f}) {};
    SpriteFrame(std::shared_ptr<Texture> texture, Size size) : m_texture(texture), m_size(size), m_texCoords({0.0f, 0.0f}, {1.0f, 1.0f}), m_offset({0.0f, 0.0f}) {};
};