#pragma once

#include "Node.hpp"
#include "Camera.hpp"

#include "Triggers.hpp"
#include "Level.hpp"
#include "SectionBatcher.hpp"
#include "Group.hpp"
#include "ColorChannel.hpp"
#include "RepeatForeverSprite.hpp"

#include <string>
#include <map>

class LevelLayer : public Node {
public:
    std::unique_ptr<Level> m_level;

    bool m_mouseDown = false;
    bool m_autoScroll = false;
    Point m_prevMousePos;

    std::vector<std::shared_ptr<ColorChannel>> m_colorChannels; //kS38
    std::map<int, std::map<float, ColorChange>> m_rawColorChanges;
    std::vector<int> m_colorChannelsWithChanges;

    std::vector<std::shared_ptr<Group>> m_groups;
    std::map<int, std::map<float, AlphaChange>> m_rawAlphaChanges;
    std::vector<int> m_groupsWithAlphaChanges;

    std::unique_ptr<GameEffect<SpeedChange>> m_speedChanges;

    std::map<std::string, std::string> m_levelProperties;
    std::vector<std::map<std::string, std::string>> m_parsedLevelString;

    std::shared_ptr<SectionBatcher> m_levelBatcher;
    std::shared_ptr<Node> m_nonBatchedObjectContainer;

    int m_groundIndex;
    int m_backgroundIndex;

    LevelLayer(Level* level);

    float timeForX(float x);

    void parseColor(std::string colorString);
    void parseLevelString();
    void parseLevelProperties();
    void setupObjects();
    void setupTriggers();

    void update(float delta);
    void draw();
    
    void updateTriggers(float time);

    void onMouseClick(int button, int action, int mods);
    void onMouseScroll(double xoffset, double yoffset);
};