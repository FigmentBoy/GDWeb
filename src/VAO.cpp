#include "VAO.hpp"

VAO::VAO() {
    glGenVertexArrays(1, &m_id);
}

void VAO::linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
    bind();
	VBO.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
}

void VAO::bind() {
    if (current == m_id) return;
    glBindVertexArray(m_id);
    current = m_id;
}

void VAO::unbind() {
}

void VAO::release() {
    glDeleteVertexArrays(1, &m_id);
}