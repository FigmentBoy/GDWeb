#include "VBO.hpp"
#include <stddef.h>

VBO::VBO() {
    glGenBuffers(1, &m_id);
}

void VBO::createBuffer(GLsizeiptr size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);
}

void VBO::setBufferData(GLintptr offset, GLvoid* data, GLsizeiptr size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VBO::setVertices(GLfloat* vertices, GLsizeiptr size) {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STREAM_DRAW);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::release() {
    glDeleteBuffers(1, &m_id);
}