#include "VAO.hpp"

VAO::VAO() {
    glGenVertexArrays(1, &m_id);
}

void VAO::linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
}

void VAO::bind() {
    glBindVertexArray(m_id);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

void VAO::release() {
    glDeleteVertexArrays(1, &m_id);
}