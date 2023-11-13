#include "EBO.hpp"
#include <stddef.h>

EBO::EBO() {
    glGenBuffers(1, &m_id);
}

void EBO::createBuffer(GLsizeiptr size) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
}

void EBO::setBufferData(GLintptr offset, GLvoid* data, GLsizeiptr size) {
    bind();
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void EBO::setIndices(GLuint* indicies, GLsizeiptr size) {
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicies, GL_STATIC_DRAW);
}

void EBO::bind() {
    if (current == m_id) return;
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    current = m_id;
}

void EBO::unbind() {
}

void EBO::release() {
    glDeleteBuffers(1, &m_id);
}