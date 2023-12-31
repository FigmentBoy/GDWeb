#pragma once

#include <memory>

#include "Batcher.hpp"
#include "Texture.hpp"
#include "Node.hpp"
#include "Color.hpp"
#include "SpriteFrame.hpp"

#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

class ColorChannel;

enum class BlendingType {
    Off = 0,
    On = 1,
    Ignore = 2
};

class Sprite : public Node {
public:
    GLfloat m_groupGroupIndex = 0;

    std::shared_ptr<ColorChannel> m_colorChannel;
    bool m_hasColorDelta = false;
    HSVAColor m_colorDelta = {0, 1, 1};

    float m_alphaModifier = 1.0f;
    float m_spriteType = -1.0f;
    
    int m_textureIndex = 0;
    int m_objectIndex = -1;
    float m_batchZLayer = 0.0f;
    Batcher* m_currentBatcher = nullptr;

    std::unique_ptr<VAO> m_vao;
    std::unique_ptr<VBO> m_vbo;
    std::unique_ptr<EBO> m_ebo;
    bool m_setup = false;
    
    GLfloat m_verticies[52];
    GLuint m_indicies[6] = {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
    };

    std::shared_ptr<SpriteFrame> m_spriteFrame;

    Sprite(std::shared_ptr<SpriteFrame> spriteFrame) : Sprite(spriteFrame, RGBAColor {1.0f, 1.0f, 1.0f, 1.0f}) {};
    Sprite(std::shared_ptr<SpriteFrame> spriteFrame, RGBAColor color);

    void removeFromBatcher();

    void update(float delta);
    void draw();
    virtual void updateVertices(BlendingType blendingType = BlendingType::Ignore);
    
    bool m_dirty = true;
    bool m_dirtyColor = false;
};