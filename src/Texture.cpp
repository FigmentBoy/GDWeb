#include "Texture.hpp"
#include <thread>
#include "Shader.hpp"

#include "Director.hpp"
#include "stb_image.h"
#include "LoadingLayer.hpp"

#include <stdio.h>
#include <string>
#include <iostream>

Texture::Texture(const char* path, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
    int width, height, numColCh;
    unsigned char* data = stbi_load(path, &width, &height, &numColCh, 4);

    Texture(data, Size {width, height}, texType, slot, format, pixelType);

    stbi_image_free(data);
}

Texture::Texture(unsigned char* imageData, Size imageSize, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
    m_slot = slot;
    m_type = texType;
    m_format = format;

    m_size = imageSize;

    glGenTextures(1, &m_id);
    bind();

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(texType, 0, GL_RGBA, m_size.width, m_size.height, 0, format, pixelType, imageData);
    glGenerateMipmap(texType);
}

std::shared_ptr<Texture> Texture::queueImageTexture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, LoadingLayer* layer) {
    auto uncommitted = std::make_shared<UncommittedTexture>(nullptr, 0, 0, texType, slot, format, pixelType);

    int width, height, numColCh;
    unsigned char* data = stbi_load(image, &uncommitted->width, &uncommitted->height, &numColCh, 4);
    uncommitted->imageData = data;

    layer->m_mutex.lock();
    layer->m_uncommittedTextures.push_back(uncommitted);
    layer->m_mutex.unlock();

    while (!uncommitted->m_finished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    };

    stbi_image_free(uncommitted->imageData);
    return uncommitted->m_texture;
}

Texture::Texture(int width, int height, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
    m_slot = slot;
    m_type = texType;
    m_format = format;

    m_size = {width, height};
    
    glGenTextures(1, &m_id);
    bind();

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(texType, 0, format, m_size.width, m_size.height, 0, GL_RGBA, pixelType, NULL);
}

std::shared_ptr<Texture> Texture::queueDataTexture(int width, int height, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, const char* uniform, LoadingLayer* layer) {
    auto uncommitted = std::make_shared<UncommittedDataTexture>(width, height, texType, slot, format, pixelType, uniform);
    
    layer->m_mutex.lock();
    layer->m_uncommittedDataTextures.push_back(uncommitted);
    layer->m_mutex.unlock();

    while (!uncommitted->m_finished) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    };

    return uncommitted->m_texture;
}

void Texture::setSubData(const GLvoid* imageData, Size imageSize, Point offset, GLenum type) {
    bind();

    glTexSubImage2D(m_type, 0, offset.x, offset.y, imageSize.width, imageSize.height, GL_RGBA, type, imageData);
}

void Texture::setUniforms(int index) {
    if (currentMap[index] == m_id) return;

    if (currentSlot != m_slot) {
        glActiveTexture(m_slot);
        currentSlot = m_slot;
    }

    glUniform1i(glGetUniformLocation(Director::get()->m_shader->m_id, ("textures[" + std::to_string(index) + "]").c_str()), m_slot - GL_TEXTURE0);
    currentMap[index] = m_id;
}

void Texture::setUniforms(const char* name) {
    if (currentSlot != m_slot) {
        glActiveTexture(m_slot);
        currentSlot = m_slot;
    }

    glUniform1i(glGetUniformLocation(Director::get()->m_shader->m_id, name), m_slot - GL_TEXTURE0);
}

void Texture::bind() {
    if (currentSlot != m_slot) {
        glActiveTexture(m_slot);
        currentSlot = m_slot;
    }
    
    if (current == m_id) return;
    glBindTexture(m_type, m_id);
    current = m_id;
}

void Texture::unbind() {
}

void Texture::release() {
    glDeleteTextures(1, &m_id);
}