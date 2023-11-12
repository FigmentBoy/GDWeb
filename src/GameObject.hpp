#pragma once

#include "Node.hpp"
#include "SectionBatcher.hpp"
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
    std::vector<int> m_groupIDs;

    Point movePosition = {0, 0};

    int copyChannelID = 0;
};

class GameObject : public Node {
public:
    inline static int m_currIndex = 0;
    static json m_gameObjectsJson;
    static void loadGameObjectsJson();

    bool m_dirtyAlpha = false;
    bool m_dirtyPosition = false;

    LevelLayer* m_layer = nullptr;

    int m_id = -1;
    int m_index = -1;
    int m_zLayer = 0;
    int m_colorChannel1 = -1;
    int m_colorChannel2 = -1;
    float m_defaultZOrder = 0.0f; // Used for ordering things on the same z layer and order

    Point m_originalPosition;

    std::vector<std::shared_ptr<Group>> m_groups;
    std::vector<std::shared_ptr<Sprite>> m_sprites;

    GameObject(int id, std::map<std::string, std::string> const& obj = {}, LevelLayer* layer = nullptr);

    void addChildSprite(std::shared_ptr<Sprite> parent, json children);
    void addToGroups();
    
    void update(float delta);
};