#pragma once

#include <GL/glew.h>

class VBO {
public:
    GLuint m_id;
    
    VBO();
    ~VBO() { release(); }
    
    void createBuffer(GLsizeiptr size);
    void setBufferData(GLintptr offset, GLvoid* data, GLsizeiptr size);

    void setVertices(GLfloat* vertices, GLsizeiptr size);

    void bind();
    void unbind();
    void release();
};