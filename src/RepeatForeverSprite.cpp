#include "RepeatForeverSprite.hpp"
#include "Director.hpp"

#include <iostream>

void RepeatForeverSprite::updateVertices() {
    if (!m_dirty) return;
    m_dirty = false;
    m_dirtyColor = false;

    Point offset = {m_spriteFrame->m_offset.x, m_spriteFrame->m_offset.y};

    Rect viewportRect = Rect(offset, m_size);
 
    float ratio = m_size.width / m_size.height;

    // This node shouldn't ever be rotated or anything so this is fineeeeeee
    Point bottomLeft = viewportRect.bottomLeft();
    glm::vec2 bottomLeftTransformed = glm::vec2(m_modelMatrix * glm::vec4(bottomLeft.x, bottomLeft.y, 0.0f, 1.0f));

    float unitWidth = m_lockedHeight * ratio;

    // We shouldn't have rotations and all on one of these sprites so this is fine
    auto camera = Director::get()->m_camera;

    float width = (camera->m_position.x + (camera->m_viewSize.x * camera->m_viewScale.x)) - bottomLeftTransformed.x;
    GLfloat times = std::ceil(width / unitWidth) + 1;

    float deltaWidth = unitWidth * times;

    float slot = m_spriteFrame->m_texture->m_slot - GL_TEXTURE0;
    float checkVal = m_hasColorDelta ? ((m_colorDelta.addS ? 1.f : 0.f) + (m_colorDelta.addV ? 0.5f : 1.f)) : 0.0f;
    
    GLfloat verticies[44] = { 
        bottomLeftTransformed.x,               bottomLeftTransformed.y,                    	   0,      1,    m_colorChannel,  m_groupGroupIndex,  slot,  m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v, checkVal, // Lower left corner
        bottomLeftTransformed.x,               bottomLeftTransformed.y + m_lockedHeight,       0,      0,    m_colorChannel,  m_groupGroupIndex,  slot,  m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v, checkVal, // Upper left corner
        bottomLeftTransformed.x + deltaWidth,  bottomLeftTransformed.y + m_lockedHeight,       times,  0,    m_colorChannel,  m_groupGroupIndex,  slot,  m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v, checkVal, // Upper right corner
        bottomLeftTransformed.x + deltaWidth,  bottomLeftTransformed.y,                        times,  1,    m_colorChannel,  m_groupGroupIndex,  slot,  m_colorDelta.h / 360.f,  m_colorDelta.s,  m_colorDelta.v, checkVal, // Lower right corner
    };

    for (int i = 0; i < 44; i++) m_verticies[i] = verticies[i];

    if (!m_currentBatcher) m_vbo->setVertices(m_verticies, sizeof(m_verticies));
}