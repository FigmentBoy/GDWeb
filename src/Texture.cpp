#include "Texture.hpp"

#include "Shader.hpp"

#include "Director.hpp"
#include "stb_image.h"
#include "LoadingLayer.hpp"


#include <stdio.h>
#include <string>
#include <iostream>

void Texture::getImageData(const char* image, LoadingLayer* layer) {
    int width, height, numColCh;
    layer->m_imageData = stbi_load(image, &width, &height, &numColCh, 4);
    layer->m_imageSize = {width, height};
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

    stbi_image_free(imageData);
}

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType) {
    m_slot = slot;
    m_type = texType;
    m_format = format;

    int width, height, numColCh;
    unsigned char* bytes = stbi_load(image, &width, &height, &numColCh, 4);

    m_size = {width, height};
    
    glGenTextures(1, &m_id);
    bind();

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(texType, 0, GL_RGBA, m_size.width, m_size.height, 0, format, pixelType, bytes);
    glGenerateMipmap(texType);

    stbi_image_free(bytes);
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

    glTexImage2D(texType, 0, GL_RGBA, m_size.width, m_size.height, 0, format, pixelType, NULL);
}

void Texture::setSubData(const GLvoid* imageData, Size imageSize, Point offset, GLenum type) {
    bind();

    glTexSubImage2D(m_type, 0, offset.x, offset.y, imageSize.width, imageSize.height, m_format, type, imageData);
    glGenerateMipmap(m_type);
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