#pragma once

#include <GL/glew.h>

class Shader {
public:
    GLuint m_id;
    Shader(const char* vertexFile, const char* fragmentFile);

    inline static GLuint current = -1;
    void activate();
    void deactivate();
    
    void release();
};