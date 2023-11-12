#include "Shader.hpp"
#include "utils.hpp"

Shader::Shader(const char* vertexFile, const char* fragmentFile) {    
    std::string vertexStr = fileContents(vertexFile);
    std::string fragmentStr = fileContents(fragmentFile);

    const char* vertexCode = vertexStr.c_str();
    const char* fragmentCode = fragmentStr.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexCode, NULL);
    glCompileShader(vertexShader);

    GLint compiled = 0;
    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* buf = (char*) malloc(infoLen);
            if (buf) {
                glGetShaderInfoLog(vertexShader, infoLen, NULL, buf);
                printf("Could not compile shader: %s\n", buf);
                free(buf);
            }
        }
        glDeleteShader(vertexShader); // Delete the shader program
        vertexShader = 0;
        return;
    }


    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
    glCompileShader(fragmentShader);

    compiled = 0;
    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* buf = (char*) malloc(infoLen);
            if (buf) {
                glGetShaderInfoLog(fragmentShader, infoLen, NULL, buf);
                printf("Could not compile shader: %s\n", buf);
                free(buf);
            }
        }
        glDeleteShader(fragmentShader); // Delete the shader program
        fragmentShader = 0;
        return;
    }

    m_id = glCreateProgram();

    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);

    GLint linked = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen) {
            char* buf = (char*) malloc(infoLen);
            if (buf) {
                glGetProgramInfoLog(m_id, infoLen, NULL, buf);
                printf("Could not link shader: %s\n", buf);
                free(buf);
            }
        }
        glDeleteProgram(m_id);
        m_id = 0;
        return;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);    
}

void Shader::activate() {
    if (current == m_id) return;
    glUseProgram(m_id);
    current = m_id;
}

void Shader::deactivate() {
}

void Shader::release() {
    glDeleteProgram(m_id);
}