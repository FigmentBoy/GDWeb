#include "GameObject.hpp"

#include "utils.hpp"
#include "Sprite.hpp"
#include "LevelLayer.hpp"
#include "SpriteFrameCache.hpp"

json GameObject::m_gameObjectsJson;

void GameObject::loadGameObjectsJson() {
    std::ifstream i("static/objects.json");
    m_gameObjectsJson = json::parse(i);
}

void GameObject::setupColorTrigger(int channel) {
    if (m_properties->copyChannelID != -1) {
        m_layer->m_rawColorChanges[channel].insert({m_position.x, ColorChange::copyColorChange(m_layer->m_colorChannels[m_properties->copyChannelID], m_properties->copyChannelDelta, std::max(0.0f, m_properties->duration), m_properties->toColorBlending, m_properties->copyOpacity, m_properties->toColor.a)});
    } else {
        m_layer->m_rawColorChanges[channel].insert({m_position.x, std::make_shared<ColorChange>(ColorChannelValue {m_properties->toColor, m_properties->toColorBlending}, std::max(0.0f, m_properties->duration))});
    }
}

GameObject::GameObject(int id, std::map<std::string, std::string> const& obj, LevelLayer* layer) {
    if (m_gameObjectsJson.empty()) {
        loadGameObjectsJson();
    }
    m_properties = std::make_unique<GameObjectProperties>();

    m_index = m_currIndex++;

    m_id = id;
    bool hasZOrder = false;
    bool hasZLayer = false;

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
                m_properties->toColor.r = std::stoi(value) / 255.f;
                break;
            case 8:
                m_properties->toColor.g = std::stoi(value) / 255.f;
                break;
            case 9:
                m_properties->toColor.b = std::stoi(value) / 255.f;
                break;
            case 10:
                m_properties->duration = std::stof(value);
                break;
            case 17:
                m_properties->toColorBlending = std::stoi(value);
                break;
            case 19:
                m_properties->usingOldColor = true;
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
                if (!m_properties->usingOldColor) m_colorChannel1 = std::stoi(value);
                break;
            case 22:
                if (!m_properties->usingOldColor) m_colorChannel2 = std::stoi(value);
                break;
            case 23:
                m_properties->targetChannel = std::stoi(value);
                break;
            case 24:
                m_zLayer = std::stoi(value);
                hasZLayer = true;
                break;
            case 25:
                m_zOrder = std::stoi(value);
                hasZOrder = true;
                break;
            case 28:
                m_properties->movePosition.x = std::stof(value);
                break;
            case 29:
                m_properties->movePosition.y = std::stof(value);
                break;
            case 30:
                switch (std::stoi(value)) {
                    case 1:
                        m_properties->function = getEasingFunction(EaseInOut);
                        break;
                    case 2:
                        m_properties->function = getEasingFunction(EaseIn);
                        break;
                    case 3:
                        m_properties->function = getEasingFunction(EaseOut);
                        break;
                    case 4:
                        m_properties->function = getEasingFunction(EaseInOutElastic);
                        break;
                    case 5:
                        m_properties->function = getEasingFunction(EaseInElastic);
                        break;
                    case 6:
                        m_properties->function = getEasingFunction(EaseOutElastic);
                        break;
                    case 7:
                        m_properties->function = getEasingFunction(EaseInOutBounce);
                        break;
                    case 8:
                        m_properties->function = getEasingFunction(EaseInBounce);
                        break;
                    case 9:
                        m_properties->function = getEasingFunction(EaseOutBounce);
                        break;
                    case 10:
                        m_properties->function = getEasingFunction(EaseInOutExpo);
                        break;
                    case 11:
                        m_properties->function = getEasingFunction(EaseInExpo);
                        break;
                    case 12:
                        m_properties->function = getEasingFunction(EaseOutExpo);
                        break;
                    case 13:
                        m_properties->function = getEasingFunction(EaseInOutSine);
                        break;
                    case 14:
                        m_properties->function = getEasingFunction(EaseInSine);
                        break;
                    case 15:
                        m_properties->function = getEasingFunction(EaseOutSine);
                        break;
                    case 16:
                        m_properties->function = getEasingFunction(EaseInOutBack);
                        break;
                    case 17:
                        m_properties->function = getEasingFunction(EaseInBack);
                        break;
                    case 18:
                        m_properties->function = getEasingFunction(EaseOutBack);
                        break;
                }
                break;
            case 32:
                m_scale = Point {1, 1} * std::stof(value);
                break;
            case 35:
                m_properties->toColor.a = std::stof(value);
                break;
            case 41:
                m_properties->hasDelta1 = true;
                break;
            case 42:
                m_properties->hasDelta2 = true;
                break;
            case 43: {
                std::vector<std::string> hsvaSplit = split(value, "a");
                m_properties->delta1.h = std::stof(hsvaSplit[0]);
                m_properties->delta1.s = std::stof(hsvaSplit[1]);
                m_properties->delta1.v = std::stof(hsvaSplit[2]);

                m_properties->delta1.addS = std::stoi(hsvaSplit[3]);
                m_properties->delta1.addV = std::stoi(hsvaSplit[4]);
                

                break;
            }
            case 44: {
                std::vector<std::string> hsvaSplit = split(value, "a");
                m_properties->delta2.h = std::stof(hsvaSplit[0]);
                m_properties->delta2.s = std::stof(hsvaSplit[1]);
                m_properties->delta2.v = std::stof(hsvaSplit[2]);

                m_properties->delta2.addS = std::stoi(hsvaSplit[3]);
                m_properties->delta2.addV = std::stoi(hsvaSplit[4]);
                break;
            }
            case 45: 
                m_properties->fadeIn = std::max(std::stof(value), 0.0f);
                break;
            case 46:
                m_properties->hold = std::max(std::stof(value), 0.0f);
                break;
            case 47:
                m_properties->fadeOut = std::max(std::stof(value), 0.0f);
                break;
            case 48:
                m_properties->pulseType = static_cast<PulseType>(std::stoi(value));
                break;
            case 49: {
                std::vector<std::string> hsvaSplit = split(value, "a");
                m_properties->copyChannelDelta.h = std::stof(hsvaSplit[0]);
                m_properties->copyChannelDelta.s = std::stof(hsvaSplit[1]);
                m_properties->copyChannelDelta.v = std::stof(hsvaSplit[2]);

                m_properties->copyChannelDelta.addS = std::stoi(hsvaSplit[3]);
                m_properties->copyChannelDelta.addV = std::stoi(hsvaSplit[4]);
                break;
            }
            case 50:
                m_properties->copyChannelID = std::stoi(value);
                break;
            case 51:
                m_properties->targetGroup = std::stoi(value);
                break;
            case 52:
                m_properties->channelType = static_cast<PulseChannel>(std::stoi(value));
                break;
            case 56:
                m_properties->activateGroup = std::stoi(value);
                break;
            case 57:
                {
                    std::vector<std::string> groupStrings = split(value, ".");
                    for (auto& groupString : groupStrings) {
                        int groupID = std::stoi(groupString);
                        m_properties->m_groups.push_back(layer->m_groups[groupID]);
                    }
                }
                break;
            case 58:
                m_properties->lockPlayerX = std::stoi(value);
                break;
            case 60:
                m_properties->copyOpacity = std::stoi(value);
                break;
            case 62:
                m_properties->spawnTriggered = std::stoi(value);
                break;
            case 85:
                m_properties->rate = std::stof(value);
                break;
            case 86:
                m_properties->pulseExclusive = std::stoi(value);
                break;
        }
    }

    json gameObjectJson = m_gameObjectsJson[std::to_string(m_id)];

    if (!hasZLayer) m_zLayer = gameObjectJson["default_z_layer"].get<int>();
    if (!hasZOrder) m_zOrder = gameObjectJson["default_z_order"].get<int>();

    m_originalPosition = m_position;
    m_groupGroupIndex = GroupGroup::getGroupGroupIndex(m_properties->m_groups);
    m_layer = layer;

    if (!m_properties->spawnTriggered) {
        // Trigger Setup
        // We will go through and change it to be time based after all objects are loaded
        switch (m_id) {
            case 29:
                setupColorTrigger(1000);
                break;
            case 30:
                setupColorTrigger(1001);
                break;
            case 105:
                setupColorTrigger(1004);
                break;
            case 899:
                if (m_properties->targetChannel <= 0) break;
                setupColorTrigger(m_properties->targetChannel);
                break;
            case 901:
                if (m_properties->targetGroup <= 0) break;
                layer->m_rawPositionChanges[m_properties->targetGroup][m_position.x].push_back(std::make_shared<PositionChange>(PositionValue {m_properties->movePosition}, std::max(0.0f, m_properties->duration), m_properties->function, m_properties->rate, m_position.x, m_properties->lockPlayerX, layer));
                break;
            case 1006: {
                if (m_properties->channelType == PulseChannel::Group) break; // Groups hurt my brain
                auto targetChannel = layer->m_colorChannels[m_properties->targetGroup];
                layer->m_rawPulseChanges[m_properties->targetGroup][m_position.x].push_back(std::make_shared<PulseChange>(m_properties->pulseExclusive, m_properties->pulseType, m_properties->toColor, targetChannel, m_properties->copyChannelID > 0 ? layer->m_colorChannels[m_properties->copyChannelID] : targetChannel, m_properties->copyChannelDelta, m_properties->fadeIn, m_properties->hold, m_properties->fadeOut));
                break;
            }
            case 1007:
                if (m_properties->targetGroup < 0) break;
                layer->m_rawAlphaChanges[m_properties->targetGroup].insert({m_position.x, std::make_shared<AlphaChange>(AlphaValue {m_properties->toColor.a}, std::max(0.0f, m_properties->duration))});
                break;
            case 1616:
                layer->m_stopTriggerLocations[m_properties->targetGroup].push_back(m_position.x);
                break;
            case 1049:
                layer->m_rawToggleChanges[m_properties->targetGroup].insert({m_position.x, std::make_shared<ToggleChange>(ToggleValue {m_properties->activateGroup}, std::max(0.0f, m_properties->duration))});
                break;
            case 200:
                layer->m_speedChanges->m_changes.insert({m_position.x, std::make_shared<SpeedChange>(SpeedValue {251.16f})});
                layer->m_rawInverseSpeedChanges.insert({m_position.x, std::make_shared<InverseSpeedChange>(InverseSpeedValue {1.f / 251.16f})});
                break;
            case 201:
                layer->m_speedChanges->m_changes.insert({m_position.x, std::make_shared<SpeedChange>(SpeedValue {311.58f})});
                layer->m_rawInverseSpeedChanges.insert({m_position.x, std::make_shared<InverseSpeedChange>(InverseSpeedValue {1.f / 311.58f})});
                break;
            case 202:
                layer->m_speedChanges->m_changes.insert({m_position.x, std::make_shared<SpeedChange>(SpeedValue {387.42f})});
                layer->m_rawInverseSpeedChanges.insert({m_position.x, std::make_shared<InverseSpeedChange>(InverseSpeedValue {1.f / 387.42f})});
                break;
            case 203:
                layer->m_speedChanges->m_changes.insert({m_position.x, std::make_shared<SpeedChange>(SpeedValue {468.0f})});
                layer->m_rawInverseSpeedChanges.insert({m_position.x, std::make_shared<InverseSpeedChange>(InverseSpeedValue {1.f / 468.0f})});
                break;
            case 1334:
                layer->m_speedChanges->m_changes.insert({m_position.x, std::make_shared<SpeedChange>(SpeedValue {576.0f})});
                layer->m_rawInverseSpeedChanges.insert({m_position.x, std::make_shared<InverseSpeedChange>(InverseSpeedValue {1.f / 576.0f})});
                break;
        }
    }

    if (!gameObjectJson.contains("texture")) return;

    if (m_colorChannel1 < 0) m_colorChannel1 = gameObjectJson.contains("default_base_color_channel") ? gameObjectJson["default_base_color_channel"].get<int>() : 0; 
    if (m_colorChannel2 < 0) m_colorChannel2 = gameObjectJson.contains("default_detail_color_channel") ? gameObjectJson["default_detail_color_channel"].get<int>() : gameObjectJson.contains("default_base_color_channel") ? gameObjectJson["default_base_color_channel"].get<int>() : 0;

    if (gameObjectJson.contains("swap_base_detail") && gameObjectJson["swap_base_detail"].get<bool>()) {
        std::swap(m_colorChannel1, m_colorChannel2);
        std::swap(m_properties->hasDelta1, m_properties->hasDelta2);
        std::swap(m_properties->delta1, m_properties->delta2);
    }

    if (m_colorChannel2 == 1012) m_colorChannel2 = -m_colorChannel1;

    std::string texture = gameObjectJson["texture"];

    auto frame = SpriteFrameCache::get()->getSpriteFrameByName(texture);
    if (!frame) {
        m_id = -1;
        return;
    };
    
    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(frame);
    addChild(sprite);
    m_sprites.push_back(sprite);

    if (layer) {
        std::string colorType = gameObjectJson.contains("color_type") ? gameObjectJson["color_type"] : "Base";
        sprite->m_colorChannel = layer->m_colorChannels[0];
        if (colorType == "Base") {
            sprite->m_colorChannel = layer->m_colorChannels[m_colorChannel1];

            if (m_properties->hasDelta1) {
                sprite->m_hasColorDelta = true;
                sprite->m_colorDelta = m_properties->delta1;
            }
        } else if (colorType == "Detail") {
            sprite->m_colorChannel = layer->m_colorChannels[std::abs(m_colorChannel2)];
            
            if (m_properties->hasDelta2) {
                sprite->m_hasColorDelta = true;
                sprite->m_colorDelta = m_properties->delta2;
            }
        } else if (colorType == "Black") {
            sprite->m_colorChannel = layer->m_colorChannels[1010];
        }
    }

    sprite->m_groupGroupIndex = m_groupGroupIndex;
    sprite->m_batchZLayer = m_zLayer;
    sprite->m_zOrder = m_zOrder; // Only for the first sprite :)

    json children = gameObjectJson["children"];
    for (auto& child : children) {
        addChildSprite(sprite, child);
    }
    
    if (m_layer) {
        m_layer->m_levelBatcher->addBatchSprite(sprite);
    };

    m_properties.reset();
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

    sprite->m_colorChannel = m_layer->m_colorChannels[0];
    if (m_layer) {
        std::string colorType = child.contains("color_type") ? child["color_type"] : "Base";
        if (colorType == "Base") {
            sprite->m_colorChannel = m_layer->m_colorChannels[m_colorChannel1];

            if (m_properties->hasDelta1) {
                sprite->m_hasColorDelta = true;
                sprite->m_colorDelta = m_properties->delta1;
            }
        } else if (colorType == "Detail") {
            sprite->m_colorChannel = m_layer->m_colorChannels[std::abs(m_colorChannel2)];
            
            if (m_properties->hasDelta2) {
                sprite->m_hasColorDelta = true;
                sprite->m_colorDelta = m_properties->delta2;
            }
        } else if (colorType == "Black") {
            sprite->m_colorChannel = m_layer->m_colorChannels[1010];
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
    sprite->m_groupGroupIndex = m_groupGroupIndex;
    m_sprites.push_back(sprite);

    sprite->m_objectIndex = m_index;
    
    if (child.contains("children")) {
        for (auto& grandchild : child["children"]) {
            addChildSprite(sprite, grandchild);
        }
    }
}