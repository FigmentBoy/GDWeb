#pragma once

#include <GL/glew.h>
#include "Geometry.hpp"

#include <unordered_map>

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

    inline static GLuint currentSlot = -1;
    inline static GLuint current = -1;
    inline static std::unordered_map<GLuint, GLuint> currentMap;

    void bind();
    void unbind();
    void release();
};