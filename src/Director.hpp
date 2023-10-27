#pragma once

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Node.hpp"
#include "Camera.hpp"
#include "Color.hpp"

class Director {
public:
    GLFWwindow* m_window;

    Size m_windowSize;
    bool m_isFullScreen;

    std::shared_ptr<Shader> m_defaultShader;
    std::shared_ptr<Shader> m_shader;

    std::shared_ptr<Camera> m_camera;
    
    float m_prevFrameTime;
    float m_prevFPSTime;
    int m_framesSince;
    float m_fpsValue;

    Point m_mousePosition;

    std::vector<Node*> m_mouseClickListeners; // We want no ownership over these at all!
    std::vector<Node*> m_mouseScrollListeners;

    std::unique_ptr<Node> m_rootNode;
    
    static Director* get() {
        static std::unique_ptr<Director> m_instance(new Director());
        return m_instance.get();
    };

    void swapRootNode(Node* node) {
        m_rootNode.reset(node);
    }

    bool init();
    void mainLoop();
};