#include "Batcher.hpp"
#include "Director.hpp"
#include "Sprite.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <iostream>

Batcher::Batcher() {
    m_vao = std::make_unique<VAO>();
    m_vao->bind();
    
    m_vbo = std::make_unique<VBO>();
    m_ebo = std::make_unique<EBO>();

    m_vao->linkAttrib(*m_vbo, 0, 2, GL_FLOAT, 10 * sizeof(GLfloat), (void*)0);
	m_vao->linkAttrib(*m_vbo, 1, 2, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
	m_vao->linkAttrib(*m_vbo, 2, 4, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
	m_vao->linkAttrib(*m_vbo, 3, 1, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(8 * sizeof(GLfloat)));
	m_vao->linkAttrib(*m_vbo, 4, 1, GL_FLOAT, 10 * sizeof(GLfloat), (void*)(9 * sizeof(GLfloat)));
}

void Batcher::addBatchSprite(std::shared_ptr<Sprite> sprite) {
    if (sprite->m_currentBatcher == this) return;
    sprite->removeFromBatcher();

    if (std::find(m_textures.begin(), m_textures.end(), sprite->m_spriteFrame->m_texture) == m_textures.end()) {
        m_textures.push_back(sprite->m_spriteFrame->m_texture);
    }
    m_sprites.push_back(sprite);
    m_sorted = false;

    recurseAdd(sprite);
}

void Batcher::removeBatchSprite(std::shared_ptr<Sprite> sprite) {
    if (sprite->m_currentBatcher != this) return;
    sprite->m_currentBatcher = nullptr;

    m_sprites.erase(std::remove(m_sprites.begin(), m_sprites.end(), sprite), m_sprites.end());

    for (auto child : sprite->m_children) {
        if (std::shared_ptr<Sprite> sprite = std::dynamic_pointer_cast<Sprite>(child)) {
            removeBatchSprite(sprite);
        }
    }
}

void Batcher::recurseAdd(std::shared_ptr<Sprite> sprite) {
    if (sprite->m_currentBatcher != nullptr) sprite->removeFromBatcher();
    sprite->m_currentBatcher = this;

    if (std::find(m_textures.begin(), m_textures.end(), sprite->m_spriteFrame->m_texture) == m_textures.end()) {
        m_textures.push_back(sprite->m_spriteFrame->m_texture);
    }

    for (auto child : sprite->m_children) {
        if (std::shared_ptr<Sprite> newSprite = std::dynamic_pointer_cast<Sprite>(child)) {
            if (newSprite->m_currentBatcher == this) newSprite->removeFromBatcher(); // So we don't draw it
            recurseAdd(newSprite);
        }
    }
}

void Batcher::draw() {
    Rect cameraRect = Director::get()->m_camera->getViewRect();
    std::vector<std::shared_ptr<Sprite>> spritesToDraw;
    int numSprites = 0;

    for (auto sprite : m_sprites) {
        if (!sprite->m_visible) continue;
        if (!sprite->getBoundingBox().intersects(cameraRect.expand(100.f))) continue;
        spritesToDraw.push_back(sprite);
        numSprites += 1 + sprite->childrenCount();
    }

    if (spritesToDraw.size() == 0) return; // No sprites to draw

    m_vao->bind();

    std::vector<GLfloat> vboData = std::vector<GLfloat>(numSprites * 10 * 4);
    std::vector<GLuint> eboData = std::vector<GLuint>(numSprites * 6);

    static GLuint templateIndices[6] = {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
    };

    int n = 0;
    for (int i = 0; i < spritesToDraw.size(); i++) {
        std::shared_ptr<Sprite> sprite = spritesToDraw[i];

        sprite->recurseChildrenWithDepth([&](std::shared_ptr<Node> node) {
            if (std::shared_ptr<Sprite> sprite = std::dynamic_pointer_cast<Sprite>(node)) {
                if (sprite->m_currentBatcher != this) return;

                GLuint indicies[6];
                for (int j = 0; j < 6; j++) {
                    indicies[j] = templateIndices[j] + (n * 4);
                }

                memcpy(vboData.data() + (n * 10 * 4), sprite->m_verticies, sizeof(GLfloat) * 10 * 4);
                memcpy(eboData.data() + (n * 6), indicies, sizeof(GLuint) * 6);

                n++;
            }
        });
    }

    if (n > prevMaxRendered) {
        m_vbo->setVertices(vboData.data(), n * sizeof(GLfloat) * 10 * 4);
        m_ebo->setIndices(eboData.data(), n * sizeof(GLuint)  * 6);
        prevMaxRendered = n;
    } else {
        m_vbo->setBufferData(0, vboData.data(), n * sizeof(GLfloat) * 10 * 4);
        m_ebo->setBufferData(0, eboData.data(), n * sizeof(GLuint)  * 6);
    }

    for (auto texture : m_textures) {
        texture->setUniforms();
        texture->bind();
    }
    
    glDrawElements(GL_TRIANGLES, n * 6, GL_UNSIGNED_INT, 0);
}