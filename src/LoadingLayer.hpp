#pragma once

#include "Node.hpp"
#include "Thread.hpp"

#include "Texture.hpp"
#include <tuple>
#include <mutex>
#include <GL/glew.h>

enum class LoadingStage {
    NONE = -1,
    SPRITES = 0,
    LEVEL = 1,
};

class LoadingLayer : public Node {
public:
    THREAD_TYPE m_thread;

    LoadingStage m_stage = LoadingStage::SPRITES;
    float m_percentDone = 0.0f;
    
    std::mutex m_mutex;
    std::vector<std::shared_ptr<UncommittedTexture>> m_uncommittedTextures;
    std::vector<std::shared_ptr<UncommittedDataTexture>> m_uncommittedDataTextures;

    LoadingLayer(bool loadResources);

    void update(float delta);
    void draw();
};