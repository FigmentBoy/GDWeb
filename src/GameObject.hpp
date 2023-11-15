#pragma once

#include "Node.hpp"
#include "Color.hpp"
#include "Sprite.hpp"
#include "easing.h"
#include <nlohmann/json.hpp>

class LevelLayer;
class Group;

using json = nlohmann::json;

struct GameObjectProperties {
    bool usingOldColor = false;

    float duration = 1.0f;
    int targetChannel = 1;
    int targetGroup = 1;
    RGBAColor toColor = {1, 1, 1, 1};
    float opacity = 1.0f;
    bool toColorBlending = false;
    std::vector<std::shared_ptr<Group>> m_groups;

    easingFunction function = [](float t, float r) { return t; };
    float rate = 1.0f;

    bool hasDelta1 = false;
    bool hasDelta2 = false;

    HSVAColor delta1;
    HSVAColor delta2;

    Point movePosition = {0, 0};

    int copyChannelID = -1;
    HSVAColor copyChannelDelta = {0, 1, 1};
};

class GameObject : public Node {
public:
    std::unique_ptr<GameObjectProperties> m_properties;

    inline static int m_currIndex = 0;
    static json m_gameObjectsJson;
    static void loadGameObjectsJson();

    bool m_dirtyAlpha = false;
    bool m_dirtyPosition = false;

    LevelLayer* m_layer = nullptr;

    int m_groupGroupIndex = 0;

    int m_id = -1;
    int m_index = -1;
    int m_zLayer = 0;
    int m_colorChannel1 = -1;
    int m_colorChannel2 = -1;
    float m_defaultZOrder = 0.0f; // Used for ordering things on the same z layer and order

    Point m_originalPosition;

    std::vector<std::shared_ptr<Sprite>> m_sprites;

    GameObject(int id, std::map<std::string, std::string> const& obj = {}, LevelLayer* layer = nullptr);

    void setupColorTrigger(int channel);

    void addChildSprite(std::shared_ptr<Sprite> parent, json children);
};