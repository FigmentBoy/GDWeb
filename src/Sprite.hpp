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
    std::shared_ptr<RGBAColor> m_color; // So we can just update them in the coloractions!
    std::shared_ptr<bool> m_blending;
    std::shared_ptr<float> m_alphaModifier;

    int m_objectIndex = -1;
    float m_batchZLayer = 0.0f;
    Batcher* m_currentBatcher = nullptr;

    std::unique_ptr<VAO> m_vao;
    std::unique_ptr<VBO> m_vbo;
    std::unique_ptr<EBO> m_ebo;
    
    GLfloat m_verticies[40];
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
    glm::mat4 muv_prevModelMatrix;
    Point muv_prevOffset;
    Size muv_prevSize;
    RGBAColor muv_prevColor;
    bool muv_prevBlending;
    float muv_prevAlphaModifier;
};