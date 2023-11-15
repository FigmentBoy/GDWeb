#include "Sprite.hpp"
#include "Director.hpp"

#include "GL/glew.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

Sprite::Sprite(std::shared_ptr<SpriteFrame> spriteFrame, RGBAColor color) {
    m_spriteFrame = spriteFrame;
    m_offset = spriteFrame->m_offset;

    m_position = {0.0f, 0.0f};

    m_size = spriteFrame->m_size;
    m_scale = {1.0f, 1.0f};

    m_anchorPoint = (Point {0.5f, 0.5f}) * m_size;
}

void Sprite::update(float delta) {
    if (!m_setup && !m_currentBatcher) {
        m_setup = true;

        m_vao = std::make_unique<VAO>();
        m_vao->bind();

        m_vbo = std::make_unique<VBO>();
        m_ebo = std::make_unique<EBO>();

        m_ebo->setIndices(m_indicies, sizeof(m_indicies));

        m_vao->linkAttrib(*m_vbo, 0, 2, GL_FLOAT, 11 * sizeof(GLfloat), (void*)0);
        m_vao->linkAttrib(*m_vbo, 1, 2, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
        m_vao->linkAttrib(*m_vbo, 2, 1, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
        m_vao->linkAttrib(*m_vbo, 3, 1, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
        m_vao->linkAttrib(*m_vbo, 4, 1, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
        m_vao->linkAttrib(*m_vbo, 5, 4, GL_FLOAT, 11 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));

        m_vao->unbind();
        m_vbo->unbind();
        m_ebo->unbind();
    }
    
    Node::update(delta);
    updateVertices();
}

void Sprite::draw() {
    if (!m_currentBatcher && m_visible) {
        if (!getBoundingBox().intersects(Director::get()->m_camera->getViewRect())) return;
        preDraw();

        m_vao->bind();
        m_vbo->bind();
        m_ebo->bind();

        m_spriteFrame->m_texture->setUniforms(m_textureIndex);
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
    if (!m_dirty) return;
    m_dirty = false;
    m_dirtyColor = false;

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

    float slot = m_textureIndex;
    float checkVal = m_hasColorDelta ? ((m_colorDelta.addS ? 1.f : 0.f) + (m_colorDelta.addV ? 0.5f : 1.f)) : 0.0f;

    if (m_spriteFrame->m_rotated) {
        GLfloat verticies[44] = {     
            bottomLeftTransformed.x,  bottomLeftTransformed.y,   	m_spriteFrame->m_texCoords.left(),  m_spriteFrame->m_texCoords.bottom(),       m_colorChannel,   m_groupGroupIndex,    slot,   m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v,     checkVal, // Lower left corner
            topLeftTransformed.x,     topLeftTransformed.y,      	m_spriteFrame->m_texCoords.right(), m_spriteFrame->m_texCoords.bottom(),       m_colorChannel,   m_groupGroupIndex,    slot,   m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v,     checkVal, // Upper left corner
            topRightTransformed.x,    topRightTransformed.y,        m_spriteFrame->m_texCoords.right(), m_spriteFrame->m_texCoords.top(),          m_colorChannel,   m_groupGroupIndex,    slot,   m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v,     checkVal, // Upper right corner
            bottomRightTransformed.x, bottomRightTransformed.y,     m_spriteFrame->m_texCoords.left(),  m_spriteFrame->m_texCoords.top(),          m_colorChannel,   m_groupGroupIndex,    slot,   m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v,     checkVal, // Lower right corner
        };

        for (int i = 0; i < 44; i++) m_verticies[i] = verticies[i];
    } else {
        GLfloat verticies[44] = { 
            bottomLeftTransformed.x,  bottomLeftTransformed.y,   	m_spriteFrame->m_texCoords.left(),  m_spriteFrame->m_texCoords.top(),          m_colorChannel,   m_groupGroupIndex,    slot,  m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v,      checkVal, // Lower left corner
            topLeftTransformed.x,     topLeftTransformed.y,      	m_spriteFrame->m_texCoords.left(),  m_spriteFrame->m_texCoords.bottom(),       m_colorChannel,   m_groupGroupIndex,    slot,  m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v,      checkVal, // Upper left corner
            topRightTransformed.x,    topRightTransformed.y,        m_spriteFrame->m_texCoords.right(), m_spriteFrame->m_texCoords.bottom(),       m_colorChannel,   m_groupGroupIndex,    slot,  m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v,      checkVal, // Upper right corner
            bottomRightTransformed.x, bottomRightTransformed.y,     m_spriteFrame->m_texCoords.right(), m_spriteFrame->m_texCoords.top(),          m_colorChannel,   m_groupGroupIndex,    slot,  m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v,      checkVal, // Lower right corner
        };

        for (int i = 0; i < 44; i++) m_verticies[i] = verticies[i];
    }

    if (!m_currentBatcher) m_vbo->setVertices(m_verticies, sizeof(m_verticies));
}