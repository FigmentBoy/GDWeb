#include "TestLayer.hpp"
#include "Director.hpp"
#include "Batcher.hpp"
#include "GameObject.hpp"
#include "Sprite.hpp"
#include "SpriteFrameCache.hpp"

TestLayer::TestLayer() {
    Director::get()->m_camera->m_position = {-128, -128};
    m_prevMousePos = Director::get()->m_mousePosition;

    auto batcher = std::make_shared<Batcher>();
    addChild(batcher);

    auto obj = std::make_shared<GameObject>(678);
    // for (auto child : obj->m_children) {
    //     if (auto sprite = std::dynamic_pointer_cast<Sprite>(child))
    //         batcher->addBatchSprite(sprite);
    // }
    addChild(obj);

    auto background = std::make_shared<Sprite>(SpriteFrameCache::get()->getSpriteFrameByName("lightBlade_01_001.png"), RGBAColor {1, 0, 0});
    background->m_position = {-64, -64};
    background->m_zOrder = -100;
    // batcher->addBatchSprite(background);
    addChild(background);

    registerMouseClickEvent();
    registerMouseScrollEvent();
}

void TestLayer::onMouseClick(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            m_mouseDown = true;
        } else if (action == GLFW_RELEASE) {
            m_mouseDown = false;
        }
    }
}

void TestLayer::onMouseScroll(double xoffset, double yoffset) {
    // Zoom in/out, offsetting the camera position so the mouse is in the same spot (satisfying!)
    auto camera = Director::get()->m_camera;

    float ratioX = (camera->m_viewScale.x - yoffset * 0.1f) / camera->m_viewScale.x;
    float ratioY = (camera->m_viewScale.y - yoffset * 0.1f) / camera->m_viewScale.y;

    camera->m_position.x += (Director::get()->m_mousePosition.x * (1.f - ratioX)) * camera->m_viewScale.x;
    camera->m_position.y += (Director::get()->m_mousePosition.y * (1.f - ratioY)) * camera->m_viewScale.y;

    camera->m_viewScale.x -= yoffset * 0.1f;
    camera->m_viewScale.y -= yoffset * 0.1f;
}

void TestLayer::update(float delta) {
    auto camera = Director::get()->m_camera;

    if (m_mouseDown) {
        camera->m_position.x -= (Director::get()->m_mousePosition.x - m_prevMousePos.x) * camera->m_viewScale.x;
        camera->m_position.y -= (Director::get()->m_mousePosition.y - m_prevMousePos.y) * camera->m_viewScale.y;
    }

    m_prevMousePos = Director::get()->m_mousePosition;

    Node::update(delta);
}