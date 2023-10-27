#include "LevelLayer.hpp"
#include "Director.hpp"
#include "utils.hpp"

#include "GameObject.hpp"
#include "GUI.hpp"

#include "GroundSprite.hpp"
#include "BackgroundSprite.hpp"

#include <GL/glew.h>

#include "SpriteFrameCache.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

LevelLayer::LevelLayer(Level* level) : m_level(level) {
    Director::get()->m_camera->m_position = {-512, -128};
    m_prevMousePos = Director::get()->m_mousePosition;

    parseLevelString();
    parseLevelProperties();

    auto backgroundSprite = new BackgroundSprite(m_backgroundIndex, this);
    backgroundSprite->m_zOrder = -1.0f;
    addChild(backgroundSprite);

    m_levelBatcher = std::make_shared<SectionBatcher>();
    m_levelBatcher->m_zOrder = 0.0f;
    addChild(m_levelBatcher);

    auto groundSprite = new GroundSprite(m_groundIndex, this);
    groundSprite->m_zOrder = 1.0f;
    addChild(groundSprite);

    setupObjects();

    std::cout << "Setting up speed changes" << std::endl;
    m_speedChanges->setup();
    setupTriggers();


    registerMouseClickEvent();
    registerMouseScrollEvent();
}

void LevelLayer::setupTriggers() {
    auto camera = Director::get()->m_camera;
    float currTime = timeForX(camera->m_position.x + camera->m_viewSize.x * camera->m_viewScale.x * 0.25f);

    std::cout << "compiling color triggers" << std::endl;
    for (auto& [channel, changes] : m_rawColorChanges) {
        m_colorChannels[channel]->m_colorTriggers = std::make_unique<GameEffect<ColorChange>>(ColorChannelValue {m_colorChannels[channel]->m_baseColor, *m_colorChannels[channel]->m_blending});
        for (auto& [position, change] : changes) {
            m_colorChannels[channel]->m_colorTriggers->m_changes.insert({timeForX(position), ColorChange {change.m_toValue, change.m_fromValue, change.m_duration}});
        }
        m_colorChannels[channel]->m_colorTriggers->setup();
        m_colorChannelsWithChanges.push_back(channel);
    }
    m_rawColorChanges.clear();

    for (auto& colorChannel : m_colorChannels) {
        colorChannel->updateColor(currTime);
    }

    std::cout << "compiling alpha triggers" << std::endl;
    for (auto& [group, changes] : m_rawAlphaChanges) {
        m_groups[group]->m_alphaTriggers = std::make_unique<GameEffect<AlphaChange>>(1.0f);
        for (auto& [position, change] : changes) {
            m_groups[group]->m_alphaTriggers->m_changes.insert({timeForX(position), AlphaChange {change.m_toValue, change.m_fromValue, change.m_duration}});
        }
        m_groups[group]->m_alphaTriggers->setup();
        m_groupsWithAlphaChanges.push_back(group);
    }
    m_rawAlphaChanges.clear();

    for (auto& group : m_groups) {
        group->updateAlphaTriggers(currTime);
    }
}

void LevelLayer::updateTriggers(float time) {
    for (auto& channel : m_colorChannelsWithChanges) {
        m_colorChannels[channel]->updateColor(time);
    }

    for (auto& group : m_groupsWithAlphaChanges) {
        m_groups[group]->updateAlphaTriggers(time);
    }
}

void LevelLayer::parseLevelString() {
    std::vector<std::string> objects = split(m_level->m_levelString, ";");
    
    std::vector<std::string> innerLevelStringSplit = split(objects[0], ",");
    for (int i = 0; i < innerLevelStringSplit.size(); i += 2) {
        m_levelProperties[innerLevelStringSplit[i]] = innerLevelStringSplit[i + 1];
    }

    for (int i = 1; i < objects.size(); i++) {
        std::map<std::string, std::string> objMap;

        std::vector<std::string> objSplit = split(objects[i], ",");

        for (int i = 0; i < objSplit.size(); i += 2) {
            objMap[objSplit[i]] = objSplit[i + 1];
        }

        m_parsedLevelString.push_back(objMap);
    }
}

void LevelLayer::parseColor(std::string colorString) {
    std::vector<std::string> channelSplit = split(colorString, "_");
    if (channelSplit.size() < 2) return;

    RGBAColor baseColor;
    int copyColor = 0;
    bool blending = false;
    int index = 1;
    HSVAColor inheritedDelta;

    for (int i = 0; i < channelSplit.size(); i += 2) {
        switch (std::stoi(channelSplit[i])) {
            case (1):
                baseColor.r = std::stoi(channelSplit[i + 1]) / 255.f;
                break;
            case (2):
                baseColor.g = std::stoi(channelSplit[i + 1]) / 255.f;
                break;
            case (3): 
                baseColor.b = std::stoi(channelSplit[i + 1]) / 255.f;
                break;
            case (4):
                // channel->m_playerColor = true;
                break;
            case (5):
                blending = true;
                break;
            case (6):
                index = std::stoi(channelSplit[i + 1]);
                break;
            case (7):
                baseColor.a = std::stof(channelSplit[i + 1]);
                break;
            case (9):
                copyColor = std::stoi(channelSplit[i + 1]);
                break;
            case (10):
                std::vector<std::string> hsvaSplit = split(channelSplit[i + 1], "a");
                inheritedDelta.h = std::stof(hsvaSplit[0]);
                inheritedDelta.s = std::stof(hsvaSplit[1]);
                inheritedDelta.v = std::stof(hsvaSplit[2]);
                inheritedDelta.sChecked = std::stoi(hsvaSplit[3]);
                inheritedDelta.vChecked = std::stoi(hsvaSplit[4]);
                break;
        }
    }

    m_colorChannels[index]->m_baseColor = baseColor;
    *m_colorChannels[index]->m_currColor = baseColor;
    *m_colorChannels[index]->m_blending = blending;
    m_colorChannels[index]->m_inheritedDelta = inheritedDelta;
    if (copyColor != 0) {
        m_colorChannels[index]->m_colorCopied = true;
        m_colorChannels[copyColor]->m_childChannels.push_back(m_colorChannels[index]);
    }
}

float LevelLayer::timeForX(float x) {
    return m_speedChanges->valueFor(x);
}

void LevelLayer::parseLevelProperties() {
    for (int i = 0; i < 1013; i++) {
        m_groups.push_back(std::make_shared<Group>());
        m_groups[i]->m_index = i;
    }

    for (int i = 0; i < 1013; i++) {
        m_colorChannels.push_back(std::make_shared<ColorChannel>());
        m_colorChannels[i]->m_index = i;
    }

    m_colorChannels[1000]->m_childChannels.push_back(m_colorChannels[1007]); // LBG (not LBJ (Lyndon B. Johnson)))
    *m_colorChannels[1007]->m_blending = true;

    *m_colorChannels[1005]->m_currColor = ColorChannel::m_p1Color; // P1
    *m_colorChannels[1006]->m_currColor = ColorChannel::m_p2Color; // P2

    *m_colorChannels[1010]->m_currColor = {0, 0, 0}; // BLACK

    for (auto& [key, value] : m_levelProperties) {
        if (key == "kA4") {
            switch (std::stoi(value)) {
                case 0:
                    m_speedChanges = std::make_unique<GameEffect<SpeedChange>>(251.16f);
                    break;
                default:
                case 1:
                    m_speedChanges = std::make_unique<GameEffect<SpeedChange>>(311.58f);
                    break;
                case 2:
                    m_speedChanges = std::make_unique<GameEffect<SpeedChange>>(387.42f);
                    break;
                case 3:
                    m_speedChanges = std::make_unique<GameEffect<SpeedChange>>(468.0f);
                    break;
                case 4:
                    m_speedChanges = std::make_unique<GameEffect<SpeedChange>>(576.0f);
                    break;
            }
        } else if (key == "kA6") {
            m_backgroundIndex = std::stoi(value);
        } else if (key == "kA7") {
            m_groundIndex = std::stoi(value);
        } else if (key == "kS1") {
            m_colorChannels[1000]->m_baseColor.r = std::stoi(value) / 255.f;
        } else if (key == "kS2") {
            m_colorChannels[1000]->m_baseColor.g = std::stoi(value) / 255.f;
        } else if (key == "kS3") {
            m_colorChannels[1000]->m_baseColor.b = std::stoi(value) / 255.f;
        } else if (key == "kS4") {
            m_colorChannels[1001]->m_baseColor.r = std::stoi(value) / 255.f;
        } else if (key == "kS5") {
            m_colorChannels[1001]->m_baseColor.g = std::stoi(value) / 255.f;
        } else if (key == "kS6") {
            m_colorChannels[1001]->m_baseColor.b = std::stoi(value) / 255.f;
        } else if (key == "kS7") {
            m_colorChannels[1002]->m_baseColor.r = std::stoi(value) / 255.f;
        } else if (key == "kS8") {
            m_colorChannels[1002]->m_baseColor.g = std::stoi(value) / 255.f;
        } else if (key == "kS9") {
            m_colorChannels[1002]->m_baseColor.b = std::stoi(value) / 255.f;
        } else if (key == "kS10") {
            m_colorChannels[1003]->m_baseColor.r = std::stoi(value) / 255.f;
        } else if (key == "kS11") {
            m_colorChannels[1003]->m_baseColor.g = std::stoi(value) / 255.f;
        } else if (key == "kS12") {
            m_colorChannels[1003]->m_baseColor.b = std::stoi(value) / 255.f;
        } else if (key == "kS13") {
            m_colorChannels[1004]->m_baseColor.r = std::stoi(value) / 255.f;
        } else if (key == "kS14") {
            m_colorChannels[1004]->m_baseColor.g = std::stoi(value) / 255.f;
        } else if (key == "kS15") {
            m_colorChannels[1004]->m_baseColor.b = std::stoi(value) / 255.f;
        }  else if (key == "kS38") {
            std::vector<std::string> colorChannelSplit = split(value, "|");
            for (auto& colorChannel : colorChannelSplit) {
                printf("parsing color channel %s\n", colorChannel.c_str());
                parseColor(colorChannel);
            }
            printf("done parsing color channels\n");
        }
    }
}

void LevelLayer::setupObjects() {
    for (auto& obj : m_parsedLevelString) {
        if (obj.find("1") == obj.end()) continue;
        int id = std::stoi(obj.at("1"));

        auto gameObj = new GameObject(id, obj, this);
        if (gameObj->m_id == -1) delete gameObj;
        addChild(gameObj);
    }
}

void LevelLayer::onMouseClick(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            m_mouseDown = true;
        } else if (action == GLFW_RELEASE) {
            m_mouseDown = false;
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            m_autoScroll = !m_autoScroll;
        }
    }
}

void LevelLayer::onMouseScroll(double xoffset, double yoffset) {
    // Zoom in/out, offsetting the camera position so the mouse is in the same spot (satisfying!)
    auto camera = Director::get()->m_camera;

    if (yoffset > 0.0f && (camera->m_viewScale.x <= 0.1f || camera->m_viewScale.y <= 0.1f)) return;

    float ratioX = (camera->m_viewScale.x - yoffset * 0.1f) / camera->m_viewScale.x;
    float ratioY = (camera->m_viewScale.y - yoffset * 0.1f) / camera->m_viewScale.y;

    float lockX = m_autoScroll ? camera->m_viewSize.x * camera->m_viewScale.x * 0.25f : Director::get()->m_mousePosition.x;

    camera->m_position.x += (lockX * (1.f - ratioX)) * camera->m_viewScale.x;
    camera->m_position.y += (Director::get()->m_mousePosition.y * (1.f - ratioY)) * camera->m_viewScale.y;

    if (camera->m_position.x < -512) camera->m_position.x = -512;
    if (camera->m_position.y < -128) camera->m_position.y = -128;

    camera->m_viewScale.x -= yoffset * 0.1f;
    camera->m_viewScale.y -= yoffset * 0.1f;
}

void LevelLayer::update(float delta) {
    Node::update(delta);

    auto camera = Director::get()->m_camera;
    float prevX = camera->m_position.x;

    if (m_autoScroll) {
        float speed = m_speedChanges->mostRecent(camera->m_position.x + camera->m_viewSize.x * camera->m_viewScale.x * 0.25f).m_toValue;
        camera->m_position.x += delta * speed;
    }

    if (m_mouseDown) {
        if (!m_autoScroll) camera->m_position.x -= (Director::get()->m_mousePosition.x - m_prevMousePos.x) * camera->m_viewScale.x;
        camera->m_position.y -= (Director::get()->m_mousePosition.y - m_prevMousePos.y) * camera->m_viewScale.y;
    
        if (camera->m_position.x < -512) camera->m_position.x = -512;
        if (camera->m_position.y < -128) camera->m_position.y = -128;
    }

    if (prevX != camera->m_position.x) {
        updateTriggers(timeForX(camera->m_position.x + camera->m_viewSize.x * camera->m_viewScale.x * 0.25f));
    }

    m_prevMousePos = Director::get()->m_mousePosition;
}

void LevelLayer::draw() {
    Node::draw();
    
    auto camera = Director::get()->m_camera;
    ImGui::SetNextWindowSize({300, 200});
    ImGui::Begin("Options");

    if (ImGui::ColorEdit4("P1 Color", &ColorChannel::m_p1Color.r)) {
        *m_colorChannels[1005]->m_currColor = ColorChannel::m_p1Color;
        m_colorChannels[1007]->parentUpdated(m_colorChannels[1000]->m_currColor->toHSVA());
    };

    if (ImGui::ColorEdit4("P2 Color", &ColorChannel::m_p2Color.r)) {
        *m_colorChannels[1006]->m_currColor = ColorChannel::m_p2Color;
    };

    ImGui::Checkbox("Auto Scroll", &m_autoScroll);

    ImGui::End();
}