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

class Sprite : public Node {
public:
    GLfloat m_colorChannel;
    GLfloat m_groupGroupIndex = 0;

    float m_alphaModifier = 1.0f;

    int m_textureIndex = 0;
    int m_objectIndex = -1;
    float m_batchZLayer = 0.0f;
    Batcher* m_currentBatcher = nullptr;

    std::unique_ptr<VAO> m_vao;
    std::unique_ptr<VBO> m_vbo;
    std::unique_ptr<EBO> m_ebo;
    
    GLfloat m_verticies[28];
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
    virtual void updateVertices();
    
    bool m_dirty = true;
    bool m_dirtyColor = false;
};