#pragma once

#include "Node.hpp"

class TestLayer : public Node {
public:
    bool m_mouseDown = false;
    Point m_prevMousePos;

    TestLayer();

    void update(float delta);

    void onMouseClick(int button, int action, int mods);
    void onMouseScroll(double xoffset, double yoffset);
};