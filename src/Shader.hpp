#pragma once

#include <GL/glew.h>

class Shader {
public:
    GLuint m_id;
    Shader(const char* vertexFile, const char* fragmentFile);

    void activate();
    void deactivate();
    
    void release();
};