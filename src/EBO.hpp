#pragma once

#include <GL/glew.h>

class EBO {
public:
    GLuint m_id;
    
    EBO();
    ~EBO() { release(); }

    void createBuffer(GLsizeiptr size);
    void setBufferData(GLintptr offset, GLvoid* data, GLsizeiptr size);

    void setIndices(GLuint* indices, GLsizeiptr size);
    void bind();
    void unbind();
    void release();
};