#pragma once

#include "Node.hpp"
#include <pthread.h>
#include "Texture.hpp"
#include <tuple>
#include <GL/glew.h>

class LoadingLayer : public Node {
public:
    pthread_t m_thread;
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