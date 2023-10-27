#pragma once

#include <glm/glm.hpp>
#include "Geometry.hpp"
#include "Node.hpp"

class Camera {
public:
    glm::vec2 m_position;
    glm::vec2 m_viewScale;
    glm::vec2 m_viewSize;

    Camera();

    Rect getViewRect();

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    glm::vec2 getPosition();
};