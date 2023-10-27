#include "Sprite.hpp"
#include "Director.hpp"

#include "GL/glew.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Sprite::Sprite(std::shared_ptr<SpriteFrame> spriteFrame, RGBAColor color) {
    m_spriteFrame = spriteFrame;
    m_offset = spriteFrame->m_offset;
    
    m_color = std::make_shared<RGBAColor>();
    *m_color = color;

    m_blending = std::make_shared<bool>(false);

    m_alphaModifier = std::make_shared<float>(1.0f);

    m_position = {0.0f, 0.0f};

    m_size = spriteFrame->m_size;
    m_scale = {1.0f, 1.0f};

    m_anchorPoint = (Point {0.5f, 0.5f}) * m_size;

    m_vao = std::make_unique<VAO>();
    m_vao->bind();

    m_vbo = std::make_unique<VBO>();
    m_ebo = std::make_unique<EBO>();

    m_ebo->setIndices(m_indicies, sizeof(m_indicies));

	m_vao->linkAttrib(*m_vbo, 0, 2, GL_FLOAT, 10 * sizeof(GLfloat), (void*)0);
	m_vao->linkAttrib(*m_vbo, 1, 2, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	m_vao->linkAttrib(*m_vbo, 2, 4, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
	m_vao->linkAttrib(*m_vbo, 3, 1, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
	m_vao->linkAttrib(*m_vbo, 4, 1, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat)));

    m_vao->unbind();
    m_vbo->unbind();
    m_ebo->unbind();
}

void Sprite::update(float delta) {
    if (m_parent != nullptr) {
        if (Sprite* m_parentSprite = dynamic_cast<Sprite*>(m_parent)) m_alphaModifier = m_parentSprite->m_alphaModifier;
    }

    Node::update(delta);
    
    updateVertices();
}

void Sprite::draw() {
    if (!m_currentBatcher && m_visible) {
        if (!getBoundingBox().intersects(Director::get()->m_camera->getViewRect())) return;
        preDraw();

        m_vao->bind();

        m_spriteFrame->m_texture->setUniforms();
        m_spriteFrame->m_texture->bind();
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        m_vao->unbind();
        m_spriteFrame->m_texture->unbind();
        
        postDraw();
    }
}

void Sprite::removeFromBatcher() {
    if (m_currentBatcher) {
        m_currentBatcher->removeBatchSprite(std::dynamic_pointer_cast<Sprite>(shared_from_this()));
    }
}

void Sprite::updateVertices() {
    if (muv_prevModelMatrix == m_modelMatrix && muv_prevColor == *m_color && muv_prevAlphaModifier == *m_alphaModifier 
        && muv_prevBlending == *m_blending  && muv_prevOffset == m_offset && muv_prevSize == m_size) return;

    muv_prevModelMatrix = m_modelMatrix;
    muv_prevOffset = m_offset;
    muv_prevSize = m_size;
    muv_prevColor = *m_color;
    muv_prevBlending = *m_blending;
    muv_prevAlphaModifier = *m_alphaModifier;

    Rect viewportRect = Rect(m_offset, m_size);
 
    Point bottomLeft = viewportRect.bottomLeft();
    Point bottomRight = viewportRect.bottomRight();
    Point topLeft = viewportRect.topLeft();
    Point topRight = viewportRect.topRight();

    // There may be a way to do this and use batchers using UBOs but I honestly can't be bothered
    glm::vec2 bottomLeftTransformed = glm::vec2(m_modelMatrix * glm::vec4(bottomLeft.x, bottomLeft.y, 0.0f, 1.0f));
    glm::vec2 bottomRightTransformed = glm::vec2(m_modelMatrix * glm::vec4(bottomRight.x, bottomRight.y, 0.0f, 1.0f));
    glm::vec2 topLeftTransformed = glm::vec2(m_modelMatrix * glm::vec4(topLeft.x, topLeft.y, 0.0f, 1.0f));
    glm::vec2 topRightTransformed = glm::vec2(m_modelMatrix * glm::vec4(topRight.x, topRight.y, 0.0f, 1.0f));

    float slot = m_spriteFrame->m_texture->m_slot - GL_TEXTURE0;
    float blendingVal = *m_blending ? 1.0f : 0.0f;

    if (m_spriteFrame->m_rotated) {
        GLfloat verticies[40] = { 
            bottomLeftTransformed.x,  bottomLeftTransformed.y,   	m_spriteFrame->m_texCoords.left(),  m_spriteFrame->m_texCoords.bottom(),       m_color->r, m_color->g, m_color->b, m_color->a * *m_alphaModifier,       slot,         blendingVal,  // Lower left corner
            topLeftTransformed.x,     topLeftTransformed.y,      	m_spriteFrame->m_texCoords.right(), m_spriteFrame->m_texCoords.bottom(),       m_color->r, m_color->g, m_color->b, m_color->a * *m_alphaModifier,       slot,         blendingVal,  // Upper left corner
            topRightTransformed.x,    topRightTransformed.y,        m_spriteFrame->m_texCoords.right(), m_spriteFrame->m_texCoords.top(),          m_color->r, m_color->g, m_color->b, m_color->a * *m_alphaModifier,       slot,         blendingVal,  // Upper right corner
            bottomRightTransformed.x, bottomRightTransformed.y,     m_spriteFrame->m_texCoords.left(),  m_spriteFrame->m_texCoords.top(),          m_color->r, m_color->g, m_color->b, m_color->a * *m_alphaModifier,       slot,         blendingVal,  // Lower right corner
        };

        for (int i = 0; i < 40; i++) m_verticies[i] = verticies[i];
    } else {
        GLfloat verticies[40] = { 
            bottomLeftTransformed.x,  bottomLeftTransformed.y,   	m_spriteFrame->m_texCoords.left(),  m_spriteFrame->m_texCoords.top(),       m_color->r, m_color->g, m_color->b, m_color->a * *m_alphaModifier,          slot,         blendingVal,  // Lower left corner
            topLeftTransformed.x,     topLeftTransformed.y,      	m_spriteFrame->m_texCoords.left(),  m_spriteFrame->m_texCoords.bottom(),    m_color->r, m_color->g, m_color->b, m_color->a * *m_alphaModifier,          slot,         blendingVal,  // Upper left corner
            topRightTransformed.x,    topRightTransformed.y,        m_spriteFrame->m_texCoords.right(), m_spriteFrame->m_texCoords.bottom(),    m_color->r, m_color->g, m_color->b, m_color->a * *m_alphaModifier,          slot,         blendingVal,  // Upper right corner
            bottomRightTransformed.x, bottomRightTransformed.y,     m_spriteFrame->m_texCoords.right(), m_spriteFrame->m_texCoords.top(),       m_color->r, m_color->g, m_color->b, m_color->a * *m_alphaModifier,          slot,         blendingVal,  // Lower right corner
        };

        for (int i = 0; i < 40; i++) m_verticies[i] = verticies[i];
    }

    if (!m_currentBatcher) m_vbo->setVertices(m_verticies, sizeof(m_verticies));
}