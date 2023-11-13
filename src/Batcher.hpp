#pragma once

#include "Node.hpp"
#include "Texture.hpp"

#include "VBO.hpp"
#include "VAO.hpp"
#include "EBO.hpp"

#include "Shader.hpp"
#include <limits>

class Sprite;

class Batcher : public Node {
public:
    GLenum m_blendMode;
    
    std::vector<std::shared_ptr<Texture>> m_textures;

    int numRendered = 0;

    std::unique_ptr<VAO> m_vao;
    std::unique_ptr<VBO> m_vbo;
    std::unique_ptr<EBO> m_ebo;

    std::vector<std::shared_ptr<Sprite>> m_sprites; // We want no ownership over these
    bool m_sorted = false;

    Batcher();

    virtual void addBatchSprite(std::shared_ptr<Sprite>);
    virtual void removeBatchSprite(std::shared_ptr<Sprite>);
    
    virtual void recurseAdd(std::shared_ptr<Sprite>);
    
    virtual void update(float dt);
    virtual void draw();
    virtual void setData();
    bool m_dirty = true;
};
