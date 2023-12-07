#pragma once

#include "Node.hpp"
#include "Camera.hpp"

#include "SpeedChange.hpp"
#include "GroupTriggers.hpp"
#include "ChannelTriggers.hpp"

#include "GameEffect.hpp"
#include "Level.hpp"
#include "Group.hpp"
#include "ColorChannel.hpp"
#include "RepeatForeverSprite.hpp"
#include "Batcher.hpp"

#include <string>
#include <map>

extern "C" {
    void play();
    void pause();
}

struct Triggers {
    std::map<int, std::map<float, std::shared_ptr<AlphaChange>>> alphaChanges;
    std::map<int, std::map<float, std::vector<std::shared_ptr<PositionChange>>>> positionChanges;
    std::map<int, std::map<float, std::shared_ptr<ToggleChange>>> toggleChanges;
    std::map<int, std::map<float, std::shared_ptr<ColorChange>>> colorChanges;
    std::map<int, std::map<float, std::vector<std::shared_ptr<PulseChange>>>> pulseChanges;
    std::map<int, std::vector<float>> stopTriggers;
    std::map<float, std::shared_ptr<InverseSpeedChange>> inverseSpeedChanges;
};

class LoadingLayer;

class LevelLayer : public Node {
public:
    static LevelLayer* m_instance;

    float m_songOffset;

    std::unique_ptr<Level> m_level;

    bool m_mouseDown = false;
    bool m_autoScroll = false;
    Point m_prevMousePos;

    Triggers m_rawTriggerData;
    Triggers m_spawnTriggerData;

    std::vector<std::shared_ptr<ColorChannel>> m_colorChannels; //kS38
    std::vector<int> m_colorChannelsWithChanges;

    std::vector<std::shared_ptr<Group>> m_groups;
    std::vector<int> m_groupsWithAlphaChanges;
    std::vector<int> m_groupsWithPositionChanges;
    std::vector<int> m_groupsWithToggleChanges;

    std::unique_ptr<GameEffect<SpeedChange>> m_speedChanges;
    std::unique_ptr<GameEffect<InverseSpeedChange>> m_inverseSpeedChanges;

    std::map<std::string, std::string> m_levelProperties;
    std::vector<std::map<std::string, std::string>> m_parsedLevelString;

    std::shared_ptr<Batcher> m_levelBatcher;
    std::shared_ptr<Node> m_nonBatchedObjectContainer;

    int m_groundIndex;
    int m_backgroundIndex;
    float m_prevX = -1000.f;

    LevelLayer(Level* level, LoadingLayer* loadingLayer=nullptr);
    LoadingLayer* m_loadingLayer = nullptr;

    float timeForX(float x);
    float xForTime(float time);

    void parseColor(std::string colorString, int base = 1);
    void parseLevelString();
    void parseLevelProperties();
    void setupObjects();
    void setupTriggers();

    void update(float delta);
    void draw();
    
    void updateTriggers(float time);
    bool m_setup = false;
    
    void onMouseClick(int button, int action, int mods);
    void onMouseScroll(double xoffset, double yoffset);

    void toggleAutoScroll();
};