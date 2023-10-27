#include "Node.hpp"
#include "Director.hpp"

#include <algorithm>
#include <memory>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Node::updateModelMatrix() {
    if (m_position == mm_prvPosition && m_rotation == mm_prvRotation && 
        m_scale == mm_prvScale  && m_flipX == mm_prvFlipX && m_flipY == mm_prvFlipY && 
        m_anchorPoint == mm_prvAnchorPoint && 
        (!m_parent || m_parent->m_parentalModelMatrix == mm_prvParentalModelMatrix)) return;

    mm_prvPosition = m_position;
    mm_prvRotation = m_rotation;
    mm_prvScale = m_scale;
    mm_prvFlipX = m_flipX;
    mm_prvFlipY = m_flipY;
    mm_prvAnchorPoint = m_anchorPoint;

    glm::mat4 translation = glm::translate(glm::mat4(1), glm::vec3(m_position.x, m_position.y, 0.0f));
    glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(-m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scale = glm::scale(glm::mat4(1), glm::vec3(m_scale.x * (m_flipX ? -1.0f : 1.0f), m_scale.y * (m_flipY ? -1.0f : 1.0f), 1.0f));
    glm::mat4 anchor = glm::translate(glm::mat4(1), glm::vec3(-m_anchorPoint.x, -m_anchorPoint.y, 0.0f));

    if (m_parent != nullptr) {
        mm_prvParentalModelMatrix = m_parent->m_parentalModelMatrix;
        m_parentalModelMatrix = m_parent->m_parentalModelMatrix * translation * rotation * scale;
        m_modelMatrix = m_parent->m_parentalModelMatrix * translation * rotation * scale * anchor;
    } else {
        m_parentalModelMatrix = translation * rotation * scale;
        m_modelMatrix = translation * rotation * scale * anchor;
    }
}

void Node::updateUniforms() {
    // GLuint matLoc = glGetUniformLocation(Director::get()->m_shader->m_id, "ModelViewMatrix");
    // Director::get()->m_shader->activate();
    // glUniformMatrix4fv(matLoc, 1, GL_TRUE, glm::value_ptr(m_modelMatrix));
}

void Node::preDraw() {
    if (!m_visible) return;

    std::sort(m_children.begin(), m_children.end(), [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
        return a->m_zOrder < b->m_zOrder;
    });

    int i;
    for (i = 0; i < m_children.size(); i++) {
        if (m_children[i]->m_zOrder >= 0) break;
        m_children[i]->draw();
    }

    m_nextDrawIndex = i;

    updateUniforms();
}

Rect Node::getBoundingBox() {
    Rect boundingBox;

    if (m_modelMatrix == mb_prvModelMatrix && m_offset == mb_prvOffset && m_size == mb_prvSize) 
            boundingBox = mb_prvBoundingBox;
    
    else {
        Rect viewportRect = Rect(m_offset, m_size);
 
        Point bottomLeft = viewportRect.bottomLeft();
        Point bottomRight = viewportRect.bottomRight();
        Point topLeft = viewportRect.topLeft();
        Point topRight = viewportRect.topRight();

        // There may be a way to do this and use batchers using UBOs but I honestly can't be bothered
        glm::vec2 bottomLeftTransformed = glm::vec2(m_modelMatrix * glm::vec4(bottomLeft.x, bottomLeft.y, 0.0f, 1.0f));
        glm::vec2 bottomRightTransformed = glm::vec2(m_modelMatrix * glm::vec4(bottomRight.x, bottomRight.y, 0.0f, 1.0f));
        glm::vec2 topLeftTransformed = glm::vec2(m_modelMatrix * glm::vec4(topLeft.x, topLeft.y, 0.0f, 1.0f));
        glm::vec2 topRightTransformed = glm::vec2(m_modelMatrix * glm::vec4(topRight.x, topRight.y, 0.0f, 1.0f));

        float minX = std::min(std::min(std::min(bottomLeftTransformed.x, bottomRightTransformed.x), topLeftTransformed.x), topRightTransformed.x);
        float minY = std::min(std::min(std::min(bottomLeftTransformed.y, bottomRightTransformed.y), topLeftTransformed.y), topRightTransformed.y);
        float maxX = std::max(std::max(std::max(bottomLeftTransformed.x, bottomRightTransformed.x), topLeftTransformed.x), topRightTransformed.x);
        float maxY = std::max(std::max(std::max(bottomLeftTransformed.y, bottomRightTransformed.y), topLeftTransformed.y), topRightTransformed.y);

        boundingBox = Rect({minX, minY}, {maxX - minX, maxY - minY});
        mb_prvBoundingBox = boundingBox;
    }
    
    for (auto& child : m_children) {
        if (child->getBoundingBox().left() < boundingBox.left()) {
            boundingBox.size.width += boundingBox.left() - child->getBoundingBox().left();
            boundingBox.position.x = child->getBoundingBox().left();
        }
        if (child->getBoundingBox().bottom() < boundingBox.bottom()) {
            boundingBox.size.height += boundingBox.bottom() - child->getBoundingBox().bottom();
            boundingBox.position.y = child->getBoundingBox().bottom();
        };
        if (child->getBoundingBox().right() > boundingBox.right()) {
            boundingBox.size.width = child->getBoundingBox().right() - boundingBox.left();
        }
        if (child->getBoundingBox().top() > boundingBox.top()) {
            boundingBox.size.height = child->getBoundingBox().top() - boundingBox.bottom();
        }
    }
    
    return boundingBox;
}

void Node::postDraw() {
    if (!m_visible) return;

    for (int i = m_nextDrawIndex; i < m_children.size(); i++) {
        m_children[i]->draw();
    }
}

void Node::draw() {
    if (!m_visible) return;

    std::sort(m_children.begin(), m_children.end(), [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
        return a->m_zOrder < b->m_zOrder;
    });

    for (auto& child : m_children) {
        child->draw();
    }
}

int Node::childrenCount() {
    int i = 0;
    recurseChildren([&i](std::shared_ptr<Node> node) {
        i++;
        return true;
    });
    return i;
}

void Node::recurseChildren(std::function<bool(std::shared_ptr<Node>)> func) {
    for (auto& child : m_children) {
        if (func(child)) child->recurseChildren(func);
    }
}

void Node::recurseChildrenWithDepth(std::function<void(std::shared_ptr<Node>)> func) {
    std::sort(m_children.begin(), m_children.end(), [](const std::shared_ptr<Node>& a, const std::shared_ptr<Node>& b) {
        return a->m_zOrder < b->m_zOrder;
    });

    int i;
    for (i = 0; i < m_children.size(); i++) {
        if (m_children[i]->m_zOrder >= 0) break;
        m_children[i]->recurseChildrenWithDepth(func);
    }

    func(shared_from_this());

    for (; i < m_children.size(); i++) {
        m_children[i]->recurseChildrenWithDepth(func);
    }
}

void Node::update(float delta) {
    updateModelMatrix();
    for (auto& child : m_children) {
        child->update(delta);
    }
}

void Node::addChild(Node* child) {
    child->removeFromParent();
    m_children.push_back(std::shared_ptr<Node>(child));
    child->m_parent = this;

    int delta = child->m_nodeDepth - (m_nodeDepth + 1);
    child->m_nodeDepth = m_nodeDepth + 1;
    
    child->recurseChildren([delta](std::shared_ptr<Node> node) {
        node->m_nodeDepth += delta;
        return true;
    });
}

void Node::addChild(std::shared_ptr<Node> child) {
    child->removeFromParent();
    m_children.push_back(child);
    child->m_parent = this;
    
    int delta = child->m_nodeDepth - (m_nodeDepth + 1);
    child->m_nodeDepth = m_nodeDepth + 1;
    
    child->recurseChildren([delta](std::shared_ptr<Node> node) {
        node->m_nodeDepth += delta;
        return true;
    });
}

void Node::removeFromParent() {
    m_parent->m_children.erase(std::remove_if(m_parent->m_children.begin(), m_parent->m_children.end(), [this](std::shared_ptr<Node>& child) {
        return child.get() == this;
    }), m_parent->m_children.end());
    m_parent = nullptr;

    int delta = -m_nodeDepth;
    m_nodeDepth = 0;

    recurseChildren([delta](std::shared_ptr<Node> node) {
        node->m_nodeDepth += delta;
        return true;
    });
}

void Node::registerMouseClickEvent() {
    Director::get()->m_mouseClickListeners.push_back((Node*) this);
}

void Node::registerMouseScrollEvent() {
    Director::get()->m_mouseScrollListeners.push_back((Node*) this);
}

Node::~Node() {
    Director::get()->m_mouseClickListeners.erase(std::remove_if(Director::get()->m_mouseClickListeners.begin(), Director::get()->m_mouseClickListeners.end(), [this](Node* node) {
        return node == this;
    }), Director::get()->m_mouseClickListeners.end());

    Director::get()->m_mouseScrollListeners.erase(std::remove_if(Director::get()->m_mouseScrollListeners.begin(), Director::get()->m_mouseScrollListeners.end(), [this](Node* node) {
        return node == this;
    }), Director::get()->m_mouseScrollListeners.end());
}