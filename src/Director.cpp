#include "Director.hpp"
#include "SpriteFrameCache.hpp"

#include "TestLayer.hpp"
#include "LoadingLayer.hpp"
#include "Plist.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "GUI.hpp"
#include "utils.hpp"

#include <gl/GL.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#ifdef EMSCRIPTEN
#include <emscripten/html5.h>
#include <emscripten.h>

EM_JS(int, canvas_get_width, (), {
  return window.innerWidth;
});

EM_JS(int, canvas_get_height, (), {
  return window.innerHeight;
});
#endif

bool Director::init() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return false;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    int width, height;
    #ifdef EMSCRIPTEN
    width = canvas_get_width();
    height = canvas_get_height();
    #else
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    width = mode->width * 0.75;
    height = mode->height * 0.75;
    #endif 

    m_windowSize.width = width;
    m_windowSize.height = height;

    m_window = glfwCreateWindow(width, height, "Editor", NULL, NULL);

    if (m_window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.\n");
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);

    glfwSetMouseButtonCallback(m_window, +[](GLFWwindow* window, int button, int action, int mods) {
        if (ImGui::GetIO().WantCaptureMouse) return;
        for (auto& listener : Director::get()->m_mouseClickListeners) {
            listener->onMouseClick(button, action, mods);
        }
    });

    glfwSetScrollCallback(m_window, +[](GLFWwindow* window, double xoffset, double yoffset){
        if (ImGui::GetIO().WantCaptureMouse) return;
        for (auto& listener : Director::get()->m_mouseScrollListeners) {
            listener->onMouseScroll(xoffset, yoffset);
        }
    });

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW: %d\n", err);
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);

    #ifdef EMSCRIPTEN
    ImGui_ImplOpenGL3_Init("#version 300 es");
    auto gui = GUI::get(); (void) gui;

    m_defaultShader = std::make_shared<Shader>("static/shaders/defaultES.vert", "static/shaders/defaultES.frag");
    #else
    ImGui_ImplOpenGL3_Init("#version 330");
    auto gui = GUI::get(); (void) gui;

    m_defaultShader = std::make_shared<Shader>("static/shaders/default.vert", "static/shaders/default.frag");
    #endif
    
    m_shader = m_defaultShader;

    m_camera = std::make_shared<Camera>();
    m_camera->m_viewSize = {m_windowSize.width, m_windowSize.height};

    m_rootNode = std::make_unique<LoadingLayer>();

    m_prevFrameTime = glfwGetTime();
    m_prevFPSTime = glfwGetTime();
    m_framesSince = 0;
    return true;
}

void Director::mainLoop() {
    float delta = glfwGetTime() - m_prevFrameTime;
    m_prevFrameTime = glfwGetTime();

    glfwMakeContextCurrent(m_window);
    glClear(GL_COLOR_BUFFER_BIT);
        
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // We can control blending in the shader
    glEnable(GL_BLEND);

    m_shader->activate();
    
    int width, height;
    #ifdef EMSCRIPTEN
    width = canvas_get_width();
    height = canvas_get_height();
    #else
    glfwGetFramebufferSize(m_window, &width, &height);
    #endif 

    m_camera->m_viewSize = {width, height};
    glViewport(0, 0, width, height);

    #ifdef EMSCRIPTEN
    emscripten_set_canvas_element_size("#canvas", width, height);
    #endif

    m_windowSize.width = width;
    m_windowSize.height = height;

    double x, y;
    glfwGetCursorPos(m_window, &x, &y);

    m_mousePosition = {(float) x, height - (float) y};

    GLuint projLoc = glGetUniformLocation(Director::get()->m_shader->m_id, "ProjectionMatrix");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(m_camera->getProjectionMatrix()));

    m_rootNode->update(delta);
    m_rootNode->draw();

    m_shader->deactivate();

    m_framesSince++;
    if (m_prevFrameTime > m_prevFPSTime + 1) {
        m_fpsValue = m_framesSince / (m_prevFrameTime - m_prevFPSTime);
        m_prevFPSTime = m_prevFrameTime;
        m_framesSince = 0;
    }

    ImGui::PushFont(GUI::get()->micross->regular);
    ImGui::GetForegroundDrawList()->AddText(ImVec2(5, 5), ImColor(1.0f, 1.0f, 1.0f, 1.0f), string_format("FPS: %.2f", m_fpsValue).c_str());
    ImGui::PopFont();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void Director::terminate() {
    glfwTerminate();
    glfwDestroyWindow(m_window);
}