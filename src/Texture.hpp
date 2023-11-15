#pragma once

#include <GL/glew.h>
#include "Geometry.hpp"

#include <unordered_map>

class LoadingLayer;
class Texture;

struct UncommittedTexture {
    unsigned char* imageData; int width; int height; GLenum texType; GLenum slot; GLenum format; GLenum pixelType;

    std::shared_ptr<Texture> m_texture = nullptr;
    bool m_finished = false;
};

struct UncommittedDataTexture {
    int width; int height; GLenum texType; GLenum slot; GLenum format; GLenum pixelType;
    const char* uniform;

    std::shared_ptr<Texture> m_texture = nullptr;
    bool m_finished = false;
};

class LoadingLayer;

class Texture {
public:
    inline static GLenum m_nextSlot = GL_TEXTURE0;

    GLuint m_id;
    GLenum m_type;
    GLenum m_slot;
    GLenum m_format;

    Size m_size;
    
    Texture(const char* path, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
    Texture(unsigned char* imageData, Size imageSize, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
    static std::shared_ptr<Texture> queueImageTexture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, LoadingLayer* layer);

    Texture(int width, int height, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);
    static std::shared_ptr<Texture> queueDataTexture(int width, int height, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, const char* uniform, LoadingLayer* loadingLayer);

    void setSubData(const GLvoid* imageData, Size imageSize, Point offset, GLenum type);

    ~Texture() { release(); }

    void setUniforms(int index);
    void setUniforms(const char* name);

    inline static GLuint currentSlot = -1;
    inline static GLuint current = -1;
    inline static std::unordered_map<GLuint, GLuint> currentMap;

    void bind();
    void unbind();
    void release();
};