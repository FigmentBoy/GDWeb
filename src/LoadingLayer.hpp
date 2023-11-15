#pragma once

#include "Node.hpp"

#ifndef _WIN32
#include <pthread.h>
#define THREAD_TYPE pthread_t
#define THREAD_CREATE(variable, function, args) pthread_create(&variable, NULL, function, args);
#define THREAD_JOIN(variable) pthread_join(variable, NULL);
#else
#include <thread>
#define THREAD_TYPE std::thread
#define THREAD_CREATE(variable, function, args) variable = std::thread(function, args);
#define THREAD_JOIN(variable) variable.join();
#endif

#include "Texture.hpp"
#include <tuple>
#include <mutex>
#include <GL/glew.h>

enum class LoadingStage {
    SPRITES,
    LEVEL
};

class LevelLayer;

class LoadingLayer : public Node {
public:
    THREAD_TYPE m_thread;

    LoadingStage m_stage = LoadingStage::SPRITES;
    float m_percentDone = 0.0f;

    bool m_done = false;
    LevelLayer* m_levelLayer;
    
    std::mutex m_mutex;
    std::vector<std::shared_ptr<UncommittedTexture>> m_uncommittedTextures;
    std::vector<std::shared_ptr<UncommittedDataTexture>> m_uncommittedDataTextures;

    LoadingLayer();

    void update(float delta);
    void draw();
};