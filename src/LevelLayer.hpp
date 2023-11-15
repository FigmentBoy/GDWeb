#pragma once

#include "Node.hpp"
#include "Camera.hpp"

#include "GameEffect.hpp"
#include "Level.hpp"
#include "Triggers.hpp"
#include "Group.hpp"
#include "ColorChannel.hpp"
#include "RepeatForeverSprite.hpp"
#include "Batcher.hpp"

#include <string>
#include <map>

class LoadingLayer;

class LevelLayer : public Node {
public:
    std::unique_ptr<Level> m_level;

    bool m_mouseDown = false;
    bool m_autoScroll = false;
    Point m_prevMousePos;

    std::vector<std::shared_ptr<ColorChannel>> m_colorChannels; //kS38
    std::map<int, std::map<float, std::shared_ptr<ColorChange>>> m_rawColorChanges;
    std::vector<int> m_colorChannelsWithChanges;

    std::vector<std::shared_ptr<Group>> m_groups;
    
    std::map<int, std::map<float, std::shared_ptr<AlphaChange>>> m_rawAlphaChanges;
    std::vector<int> m_groupsWithAlphaChanges;

    std::map<int, std::map<float, std::vector<std::shared_ptr<PositionChange>>>> m_rawPositionChanges;
    std::vector<int> m_groupsWithPositionChanges;

    std::map<int, std::map<float, std::shared_ptr<ToggleChange>>> m_rawToggleChanges;
    std::vector<int> m_groupsWithToggleChanges;

    std::map<int, std::map<float, std::vector<std::shared_ptr<PulseChange>>>> m_rawPulseChanges;
    std::unique_ptr<GameEffect<PulseChange>> m_pulseChanges;
    std::vector<int> m_colorChannelsWithPulseChanges;

    std::unique_ptr<GameEffect<SpeedChange>> m_speedChanges;

    std::map<float, std::shared_ptr<InverseSpeedChange>> m_rawInverseSpeedChanges;
    std::unique_ptr<GameEffect<InverseSpeedChange>> m_inverseSpeedChanges;

    std::map<std::string, std::string> m_levelProperties;
    std::vector<std::map<std::string, std::string>> m_parsedLevelString;

    std::shared_ptr<Batcher> m_levelBatcher;
    std::shared_ptr<Node> m_nonBatchedObjectContainer;

    std::map<int, std::vector<float>> m_stopTriggerLocations;

    int m_groundIndex;
    int m_backgroundIndex;
    float m_prevX = -1000.f;

    LevelLayer(Level* level, LoadingLayer* loadingLayer=nullptr);
    LoadingLayer* m_loadingLayer = nullptr;

    float timeForX(float x);
    float xForTime(float time);

    void parseColor(std::string colorString);
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
};