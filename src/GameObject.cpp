#include "GameObject.hpp"

#include "utils.hpp"
#include "Sprite.hpp"
#include "LevelLayer.hpp"
#include "SpriteFrameCache.hpp"
#include <fstream>

json GameObject::m_gameObjectsJson;

void GameObject::loadGameObjectsJson() {
    std::ifstream i("static/objects.json");
    m_gameObjectsJson = json::parse(i);
}

GameObject::GameObject(int id, std::map<std::string, std::string> const& obj, LevelLayer* layer) {
    if (m_gameObjectsJson.empty()) {
        loadGameObjectsJson();
    }
    std::unique_ptr<GameObjectProperties> properties = std::make_unique<GameObjectProperties>();

    m_index = m_currIndex++;

    m_id = id;
    for (auto& [key, value] : obj) {
        switch(std::stoi(key)) {
            case 1:
                m_id = std::stoi(value);
                break;
            case 2:
                m_position.x = std::stof(value);
                break;
            case 3:
                m_position.y = std::stof(value);
                break;
            case 4:
                m_flipX = std::stoi(value);
                break;
            case 5:
                m_flipY = std::stoi(value);
                break;
            case 6:
                m_rotation = std::stof(value);
                break;
            case 7:
                properties->toColor.r = std::stoi(value) / 255.f;
                break;
            case 8:
                properties->toColor.g = std::stoi(value) / 255.f;
                break;
            case 9:
                properties->toColor.b = std::stoi(value) / 255.f;
                break;
            case 10:
                properties->duration = std::stof(value);
                break;
            case 17:
                properties->toColorBlending = std::stoi(value);
                break;
            case 19:
                properties->usingOldColor = true;
                switch (std::stoi(value)) {
                    case 1:
                        m_colorChannel1 = 1005;
                        m_colorChannel2 = 1005;
                        break;
                    case 2:
                        m_colorChannel1 = 1006;
                        m_colorChannel2 = 1006;
                        break;
                    case 3:
                        m_colorChannel1 = 1;
                        m_colorChannel2 = 1;
                        break;
                    case 4:
                        m_colorChannel1 = 2;
                        m_colorChannel2 = 2;
                        break;
                    case 5:
                        m_colorChannel1 = 1007;
                        m_colorChannel2 = 1007;
                        break;
                    case 6:
                        m_colorChannel1 = 3;
                        m_colorChannel2 = 3;
                        break;
                    case 7:
                        m_colorChannel1 = 4;
                        m_colorChannel2 = 4;
                        break;
                    case 8:
                        m_colorChannel1 = 1003;
                        m_colorChannel2 = 1003;
                        break;
                }
                break;
            case 21:
                if (!properties->usingOldColor) m_colorChannel1 = std::stoi(value);
                break;
            case 22:
                if (!properties->usingOldColor) m_colorChannel2 = std::stoi(value);
                break;
            case 23:
                properties->targetChannel = std::stoi(value);
                break;
            case 24:
                m_zLayer = std::stoi(value);
                break;
            case 25:
                m_zOrder = std::stoi(value);
                break;
            case 28:
                properties->movePosition.x = std::stof(value);
                break;
            case 29:
                properties->movePosition.y = std::stof(value);
                break;
            case 32:
                m_scale = Point {1, 1} * std::stof(value);
                break;
            case 35:
                properties->opacity = std::stof(value);
                break;
            case 50:
                properties->copyChannelID = std::stoi(value);
                break;
            case 51:
                properties->targetGroup = std::stoi(value);
                break;
            case 57:
                std::vector<std::string> groupStrings = split(value, ".");
                for (auto& groupString : groupStrings) {
                    int groupID = std::stoi(groupString);
                    properties->m_groupIDs.push_back(groupID);
                }
                break;
        }
    }

    m_originalPosition = m_position;

    for (auto group : properties->m_groupIDs) {
        m_groups.push_back(layer->m_groups[group]);
    }

    updateModelMatrix();

    // Trigger Setup
    // We will go through and change it to be time based after all objects are loaded
    switch (m_id) {
        case 29:
            layer->m_rawColorChanges[1000].insert({m_position.x, {{properties->toColor, properties->toColorBlending}, std::max(0.0f, properties->duration)}});
            break;
        case 30:
            layer->m_rawColorChanges[1001].insert({m_position.x, {{properties->toColor, properties->toColorBlending}, std::max(0.0f, properties->duration)}});
            break;
        case 105:
            layer->m_rawColorChanges[1004].insert({m_position.x, {{properties->toColor, properties->toColorBlending}, std::max(0.0f, properties->duration)}});
            break;
        case 899:
            if (properties->targetChannel <= 0) break;
            layer->m_rawColorChanges[properties->targetChannel].insert({m_position.x, {{properties->toColor, properties->toColorBlending}, std::max(0.0f, properties->duration)}});
            break;
        case 901:
            if (properties->targetGroup <= 0) break;
            layer->m_rawPositionChanges[properties->targetGroup].insert({m_position.x, {{properties->movePosition}, std::max(0.0f, properties->duration)}});
            break;
        case 1007:
            if (properties->targetGroup < 0) break;
            layer->m_rawAlphaChanges[properties->targetGroup].insert({m_position.x, {{properties->opacity}, std::max(0.0f, properties->duration)}});
            break;
        case 200:
            layer->m_speedChanges->m_changes.insert({m_position.x, {{251.16f}}});
            break;
        case 201:
            layer->m_speedChanges->m_changes.insert({m_position.x, {{311.58f}}});
            break;
        case 202:
            layer->m_speedChanges->m_changes.insert({m_position.x, {{387.42f}}});
            break;
        case 203:
            layer->m_speedChanges->m_changes.insert({m_position.x, {{468.0f}}});
            break;
        case 1334:
            layer->m_speedChanges->m_changes.insert({m_position.x, {{576.0f}}});
            break;
    }

    json gameObjectJson = m_gameObjectsJson[std::to_string(m_id)];
    if (!gameObjectJson.contains("texture")) return;
    
    if (m_colorChannel1 < 0) m_colorChannel1 = gameObjectJson.contains("default_base_color_channel") ? gameObjectJson["default_base_color_channel"].get<int>() : 0;
    if (m_colorChannel2 < 0) m_colorChannel2 = gameObjectJson.contains("default_detail_color_channel") ? gameObjectJson["default_detail_color_channel"].get<int>() : gameObjectJson.contains("default_base_color_channel") ? gameObjectJson["default_base_color_channel"].get<int>() : 0;

    if (gameObjectJson.contains("swap_base_detail") && gameObjectJson["swap_base_detail"].get<bool>()) std::swap(m_colorChannel1, m_colorChannel2);

    if (obj.find("24") == obj.end()) m_zLayer = gameObjectJson["default_z_layer"].get<int>();
    if (obj.find("25") == obj.end()) m_zOrder = gameObjectJson["default_z_order"].get<int>();

    std::string texture = gameObjectJson["texture"];

    auto frame = SpriteFrameCache::get()->getSpriteFrameByName(texture);
    if (!frame) return;
    
    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(frame);
    addChild(sprite);
    m_sprites.push_back(sprite);

    if (layer) {
        m_layer = layer;

        std::string colorType = gameObjectJson.contains("color_type") ? gameObjectJson["color_type"] : "Base";
        if (colorType == "Base") {
            sprite->m_color = m_layer->m_colorChannels[m_colorChannel1]->m_currColor;
            sprite->m_blending = m_layer->m_colorChannels[m_colorChannel1]->m_blending;
            m_layer->m_colorChannels[m_colorChannel1]->m_sprites.push_back(sprite);
        } else if (colorType == "Detail") {
            sprite->m_color = m_layer->m_colorChannels[m_colorChannel2]->m_currColor;
            sprite->m_blending = m_layer->m_colorChannels[m_colorChannel2]->m_blending;
            m_layer->m_colorChannels[m_colorChannel2]->m_sprites.push_back(sprite);
        } else if (colorType == "Black") {
            sprite->m_color = m_layer->m_colorChannels[1010]->m_currColor;
        }
    }

    sprite->updateModelMatrix();

    sprite->m_batchZLayer = m_zLayer;
    sprite->m_objectIndex = m_index;

    json children = gameObjectJson["children"];
    for (auto& child : children) {
        addChildSprite(sprite, child);
    }
    
    if (m_layer) {
        m_layer->m_levelBatcher->addBatchSprite(sprite);
    };
}

void GameObject::addToGroups() {
    for (auto group : m_groups) {
        group->m_objects.push_back(std::static_pointer_cast<GameObject>(shared_from_this()));
    }
}

void GameObject::addChildSprite(std::shared_ptr<Sprite> parent, json child) {
    std::string texture = child["texture"];   
    auto frame = SpriteFrameCache::get()->getSpriteFrameByName(texture);
    if (!frame) return;

    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(frame);
    
    Point deltaAnchor = {sprite->m_size.width * child["anchor_x"].get<float>() * (child["flip_x"].get<bool>() ? -1.f : 1.f), sprite->m_size.height * child["anchor_y"].get<float>() * (child["flip_y"].get<bool>() ? -1.f : 1.f)};
    if (sprite->m_spriteFrame->m_rotated) {
        deltaAnchor = {-deltaAnchor.y, deltaAnchor.x};
    }

    if (m_layer) {
        std::string colorType = child.contains("color_type") ? child["color_type"] : "Base";
        if (colorType == "Base") {
            sprite->m_color = m_layer->m_colorChannels[m_colorChannel1]->m_currColor;
            sprite->m_blending = m_layer->m_colorChannels[m_colorChannel1]->m_blending;
            m_layer->m_colorChannels[m_colorChannel1]->m_sprites.push_back(sprite);
        } else if (colorType == "Detail") {
            sprite->m_color = m_layer->m_colorChannels[m_colorChannel2]->m_currColor;
            sprite->m_blending = m_layer->m_colorChannels[m_colorChannel2]->m_blending;
            m_layer->m_colorChannels[m_colorChannel2]->m_sprites.push_back(sprite);
        } else if (colorType == "Black") {
            *sprite->m_color = {0.f, 0.f, 0.f, 1.f};
        }
    }
    
    sprite->m_anchorPoint += deltaAnchor;
    sprite->m_scale = Point {child["scale_x"].get<float>(), child["scale_y"].get<float>()};
    sprite->m_flipX = child["flip_x"].get<bool>();
    sprite->m_flipY = child["flip_y"].get<bool>();
    sprite->m_position = Point {child["x"].get<float>(), child["y"].get<float>()};
    sprite->m_rotation = child["rot"].get<float>();

    if (child.contains("z")) sprite->m_zOrder = child["z"].get<int>();
    
    parent->addChild(sprite);
    sprite->updateModelMatrix();
    m_sprites.push_back(sprite);

    sprite->m_batchZLayer = m_zLayer;
    sprite->m_objectIndex = m_index;
    
    if (child.contains("children")) {
        for (auto& grandchild : child["children"]) {
            addChildSprite(sprite, grandchild);
        }
    }
}

void GameObject::update(float delta) {
    if (m_dirtyAlpha) {
        m_dirtyAlpha = false;
        float alpha = 1.0f;

        for (auto group : m_groups) {
            alpha *= group->m_currAlpha;
        }

        for (auto sprite : m_sprites) {
            sprite->m_alphaModifier = alpha;
            sprite->m_dirtyColor = true;
        }
    }

    if (m_dirtyPosition) {
        m_dirtyPosition = false;
        Point delta = {0, 0};

        for (auto group : m_groups) {
            delta += group->m_position;
        }

        m_position = m_originalPosition + delta;
        m_dirtyMatrix = true;
    }

    Node::update(delta);
}