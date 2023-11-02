#pragma once

#include "Node.hpp"

#ifndef _WIN32
#include <pthread.h>
#define THREAD_TYPE pthread_t
#define THREAD_CREATE(variable, function, args) pthread_create(&variable, NULL, function, args);
#else
#include <thread>
#define THREAD_TYPE std::thread
#define THREAD_CREATE(variable, function, args) variable = std::thread(function, args);
#endif

#include "Texture.hpp"
#include <tuple>
#include <GL/glew.h>

class LoadingLayer : public Node {
public:
    THREAD_TYPE m_thread;
    float m_percentDone = 0.0f;
    bool m_doneLoading = false;
    
    bool m_hasNewImage = false;
    unsigned char* m_imageData;
    Size m_imageSize;
    GLenum m_spriteSlot = GL_TEXTURE0;
    Texture* m_texture;

    LoadingLayer();

    void update(float delta);
    void draw();
};