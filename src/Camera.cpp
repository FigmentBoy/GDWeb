#include "Camera.hpp"
#include "Director.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <limits>

Camera::Camera() {
    m_position = {0.0f, 0.0f};
    m_viewScale = {1.0f, 1.0f};
}

Rect Camera::getViewRect() {
    return Rect(m_position.x, m_position.y, m_viewSize.x * m_viewScale.x, m_viewSize.y * m_viewScale.y);
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::ortho(m_position.x, m_position.x + (m_viewSize.x * m_viewScale.x), m_position.y, m_position.y + (m_viewSize.y * m_viewScale.y), -std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
}

glm::mat4 Camera::getViewMatrix() {
    return glm::mat4(1.0f);
}

glm::vec2 Camera::getPosition() {
    return glm::vec2(m_position.x, m_position.y);
}

