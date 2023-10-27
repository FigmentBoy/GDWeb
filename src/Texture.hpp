#pragma once

#include <GL/glew.h>
#include "Geometry.hpp"

#include <tuple>

class LoadingLayer;

class Texture {
public:
    GLuint m_id;
    GLenum m_type;
    GLenum m_slot;

    Size m_size;
    
    static void getImageData(const char* image, LoadingLayer* layer);
    Texture(unsigned char* imageData, Size imageSize, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
    Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
    ~Texture() { release(); }

    void setUniforms();

    void bind();
    void unbind();
    void release();
};