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

    m_size = imageSize;

    glGenTextures(1, &m_id);
    glActiveTexture(slot);
    glBindTexture(texType, m_id);

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

    int width, height, numColCh;
    unsigned char* bytes = stbi_load(image, &width, &height, &numColCh, 4);

    m_size = {width, height};
    
    glGenTextures(1, &m_id);
    glActiveTexture(slot);
    glBindTexture(texType, m_id);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(texType, 0, GL_RGBA, m_size.width, m_size.height, 0, format, pixelType, bytes);
    glGenerateMipmap(texType);

    stbi_image_free(bytes);
}

void Texture::setUniforms() {
    glActiveTexture(m_slot);
    glUniform1i(glGetUniformLocation(Director::get()->m_shader->m_id, ("textures[" + std::to_string(m_slot - GL_TEXTURE0) + "]").c_str()), m_slot - GL_TEXTURE0);
}

void Texture::bind() {
    glActiveTexture(m_slot);
    glBindTexture(m_type, m_id);
}

void Texture::unbind() {
    glBindTexture(m_type, 0);
}

void Texture::release() {
    glDeleteTextures(1, &m_id);
}