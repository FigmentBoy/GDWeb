#include "EBO.hpp"
#include <stddef.h>

EBO::EBO() {
    glGenBuffers(1, &m_id);
}

void EBO::createBuffer(GLsizeiptr size) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW);
}

void EBO::setBufferData(GLintptr offset, GLvoid* data, GLsizeiptr size) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void EBO::setIndices(GLuint* indicies, GLsizeiptr size) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indicies, GL_STREAM_DRAW);
}

void EBO::bind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void EBO::unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::release() {
    glDeleteBuffers(1, &m_id);
}