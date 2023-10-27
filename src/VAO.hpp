#pragma once

#include <GL/glew.h>
#include "VBO.hpp"

class VAO {
public:
    GLuint m_id;
    
    VAO();
    ~VAO() { release(); }

    void linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void bind();
    void unbind();
    void release();
};