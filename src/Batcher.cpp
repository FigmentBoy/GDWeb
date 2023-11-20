#include "Batcher.hpp"
#include "Director.hpp"
#include "Sprite.hpp"
#include "ColorChannel.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <iostream>

void Batcher::addBatchSprite(std::shared_ptr<Sprite> sprite) {
    if (sprite->m_currentBatcher == this) return;
    sprite->removeFromBatcher();

    if (std::find(m_textures.begin(), m_textures.end(), sprite->m_spriteFrame->m_texture) == m_textures.end()) {
        m_textures.push_back(sprite->m_spriteFrame->m_texture);
    }

    m_sprites.push_back(sprite);

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

    int textureIndex = std::find(m_textures.begin(), m_textures.end(), sprite->m_spriteFrame->m_texture) - m_textures.begin();
    sprite->m_textureIndex = textureIndex;

    for (auto child : sprite->m_children) {
        if (std::shared_ptr<Sprite> newSprite = std::dynamic_pointer_cast<Sprite>(child)) {
            if (newSprite->m_currentBatcher == this) newSprite->removeFromBatcher(); // So we don't draw it
            recurseAdd(newSprite);
        }
    }
}

void Batcher::setData() {
    m_vao->bind();

    if (!m_dirty) return;
    m_dirty = false;

    std::map<float, std::vector<std::shared_ptr<Sprite>>> zLayers;

    for (auto sprite : m_sprites) {
        if (!sprite->m_visible) continue;
        if (sprite->m_currentBatcher != this) continue;

        zLayers[sprite->m_batchZLayer].push_back(sprite);
    }

    for (auto& [layer, sprites] : zLayers) {
        std::sort(sprites.begin(), sprites.end(), [](std::shared_ptr<Sprite> a, std::shared_ptr<Sprite> b) {
            return a->m_zOrder < b->m_zOrder;
        });
    }

    std::vector<GLfloat> vboData = std::vector<GLfloat>();
    std::vector<GLuint> eboData = std::vector<GLuint>();

    static GLuint templateIndices[6] = {
        0, 2, 1, // Upper triangle
        0, 3, 2 // Lower triangle
    };

    int n = 0;
    std::vector<std::shared_ptr<Sprite>> nonBlendingSprites;

    for (auto& [layer, sprites] : zLayers) {
        for (auto sprite : sprites) {
            if (!sprite->m_visible) continue;
            if (sprite->m_currentBatcher != this) continue;

            sprite->recurseChildrenWithDepth([&](std::shared_ptr<Node> node) {
                if (std::shared_ptr<Sprite> sprite = std::dynamic_pointer_cast<Sprite>(node)) {
                    if (!sprite->m_visible) return;
                    if (sprite->m_currentBatcher != this) return;
                        
                    GLuint indicies[6];
                    for (int j = 0; j < 6; j++) {
                        indicies[j] = templateIndices[j] + (n * 4);
                    }

                    sprite->updateVertices(BlendingType::On);

                    vboData.resize((n + 1) * sizeof(sprite->m_verticies) / sizeof(GLfloat));
                    memcpy(vboData.data() + (n * sizeof(sprite->m_verticies) / sizeof(GLfloat)), sprite->m_verticies, sizeof(GLfloat) * sizeof(sprite->m_verticies) / sizeof(GLfloat));

                    eboData.resize((n + 1) * 6);
                    memcpy(eboData.data() + (n * 6), indicies, sizeof(GLuint) * 6);

                    n++;
                }
            });
        }

        for (auto sprite : sprites) {
            if (!sprite->m_visible) continue;
            if (sprite->m_currentBatcher != this) continue;

            sprite->recurseChildrenWithDepth([&](std::shared_ptr<Node> node) {
                if (std::shared_ptr<Sprite> sprite = std::dynamic_pointer_cast<Sprite>(node)) {
                    if (!sprite->m_visible) return;
                    if (sprite->m_currentBatcher != this) return;
                        
                    GLuint indicies[6];
                    for (int j = 0; j < 6; j++) {
                        indicies[j] = templateIndices[j] + (n * 4);
                    }

                    sprite->updateVertices(BlendingType::Off);

                    vboData.resize((n + 1) * sizeof(sprite->m_verticies) / sizeof(GLfloat));
                    memcpy(vboData.data() + (n * sizeof(sprite->m_verticies) / sizeof(GLfloat)), sprite->m_verticies, sizeof(GLfloat) * sizeof(sprite->m_verticies) / sizeof(GLfloat));

                    eboData.resize((n + 1) * 6);
                    memcpy(eboData.data() + (n * 6), indicies, sizeof(GLuint) * 6);

                    n++;
                }
            });
        }
    }

    m_vbo->setVertices(vboData.data(), vboData.size() * sizeof(GLfloat));
    m_ebo->setIndices(eboData.data(), eboData.size() * sizeof(GLuint));

    numRendered = n;
}

void Batcher::update(float dt) {
    if (!m_setup) {
        m_setup = true;

        m_vao = std::make_unique<VAO>();
        m_vao->bind();
        
        m_vbo = std::make_unique<VBO>();
        m_ebo = std::make_unique<EBO>();

        m_vbo->bind();
        m_ebo->bind();

        m_vao->linkAttrib(*m_vbo, 0, 2, GL_FLOAT, 13 * sizeof(GLfloat), (void*)0);
        m_vao->linkAttrib(*m_vbo, 1, 2, GL_FLOAT, 13 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
        m_vao->linkAttrib(*m_vbo, 2, 1, GL_FLOAT, 13 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
        m_vao->linkAttrib(*m_vbo, 3, 1, GL_FLOAT, 13 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
        m_vao->linkAttrib(*m_vbo, 4, 1, GL_FLOAT, 13 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
        m_vao->linkAttrib(*m_vbo, 5, 4, GL_FLOAT, 13 * sizeof(GLfloat), (void*)(7 * sizeof(GLfloat)));
        m_vao->linkAttrib(*m_vbo, 6, 1, GL_FLOAT, 13 * sizeof(GLfloat), (void*)(11 * sizeof(GLfloat)));
        m_vao->linkAttrib(*m_vbo, 7, 1, GL_FLOAT, 13 * sizeof(GLfloat), (void*)(12 * sizeof(GLfloat)));
    }

    if (m_dirty) Node::update(dt);
    setData();
}

void Batcher::draw() {
    m_vao->bind();

    for (int i = 0; i < m_textures.size(); i++) {
        m_textures[i]->setUniforms(i);
        m_textures[i]->bind();
    }
    
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, numRendered * 6, GL_UNSIGNED_INT, 0);
}