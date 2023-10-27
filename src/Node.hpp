#pragma once

#include "Geometry.hpp"

#include <functional>
#include <vector>
#include <glm/glm.hpp>

class Node : public std::enable_shared_from_this<Node> {
public:
    Point m_position;
    Size m_size;
    float m_zOrder;
    Point m_scale;
    Point m_anchorPoint;
    float m_rotation;
    bool m_flipX;
    bool m_flipY;
    Point m_offset;
    int m_nodeDepth = 0;

    bool m_visible = true;

    int m_nextDrawIndex;

    Node* m_parent = nullptr;
    std::vector<std::shared_ptr<Node>> m_children;

    glm::mat4 m_parentalModelMatrix;
    glm::mat4 m_modelMatrix;

    Node() : m_position({0.0f, 0.0f}), m_size({1.0f, 1.0f}), m_zOrder(0.0f), m_scale({1.0f, 1.0f}), m_anchorPoint({0.f, 0.f}), m_rotation(0.0f), m_flipX(false), m_flipY(false) {};
    ~Node();

    void registerMouseClickEvent();
    void registerMouseScrollEvent();

    int childrenCount();
    void recurseChildren(std::function<bool(std::shared_ptr<Node>)> func);
    void recurseChildrenWithDepth(std::function<void(std::shared_ptr<Node>)> func);

    virtual void onMouseClick(int button, int action, int mods) {};
    virtual void onMouseScroll(double xoffset, double yoffset) {};

    virtual void updateModelMatrix();
    Point mm_prvPosition;
    float mm_prvRotation;
    Point mm_prvScale;
    bool mm_prvFlipX;
    bool mm_prvFlipY;
    Point mm_prvAnchorPoint;
    glm::mat4 mm_prvParentalModelMatrix;

    virtual void updateUniforms();

    virtual Rect getBoundingBox();
    Rect mb_prvBoundingBox;
    glm::mat4 mb_prvModelMatrix;
    Point mb_prvOffset;
    Size mb_prvSize;

    virtual void preDraw();
    virtual void postDraw();

    virtual void draw();
    virtual void update(float dt);

    virtual void addChild(Node* child);
    virtual void addChild(std::shared_ptr<Node> child);
    virtual void removeFromParent();
};