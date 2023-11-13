#include "VBO.hpp"
#include <stddef.h>

VBO::VBO() {
    glGenBuffers(1, &m_id);
}

void VBO::createBuffer(GLsizeiptr size) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
}

void VBO::setBufferData(GLintptr offset, GLvoid* data, GLsizeiptr size) {
    bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VBO::setVertices(GLfloat* vertices, GLsizeiptr size) {
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::bind() {
    if (current == m_id) return;
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    current = m_id;
}

void VBO::unbind() {
}

void VBO::release() {
    glDeleteBuffers(1, &m_id);
}