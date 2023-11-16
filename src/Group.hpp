#pragma once

#include "GameObject.hpp"
#include "GroupTriggers.hpp"

class GroupGroup;
class Group {
public:
    int m_index;
    
    Point m_position;
    float m_rotation;
    float m_currAlpha;
    bool m_toggled = true;

    std::unique_ptr<GameEffect<AlphaChange>> m_alphaTriggers;
    std::unique_ptr<GameEffect<PositionChange>> m_positionChanges;
    std::unique_ptr<GameEffect<ToggleChange>> m_toggleChanges;

    std::vector<std::shared_ptr<GroupGroup>> m_groupGroups;

    Group() : m_position(0, 0), m_rotation(0), m_currAlpha(1.f) {};

    void updateAlphaTriggers(float time);
    void updatePositionChanges(float time);
    void updateToggleChanges(float time);

    void addStopTrigger(float time);
};


class GroupGroup { // lol
public:
    std::vector<std::shared_ptr<Group>> m_groups;
    std::shared_ptr<Texture> m_groupTexture;

    int m_index;

    GroupGroup(std::vector<std::shared_ptr<Group>> const& groups) : m_groups(groups) {};

    inline static std::vector<std::shared_ptr<GroupGroup>> m_groupGroups = {std::make_shared<GroupGroup>(std::vector<std::shared_ptr<Group>>())};

    static int getGroupGroupIndex(std::vector<std::shared_ptr<Group>> const& groups) {
        for (auto& groupGroup : m_groupGroups) {
            if (std::is_permutation(groupGroup->m_groups.begin(), groupGroup->m_groups.end(), groups.begin(), groups.end())) {
                return groupGroup->m_index;
            }
        }

        auto groupGroup = std::make_shared<GroupGroup>(groups);

        for (auto& group : groups) {
            group->m_groupGroups.push_back(groupGroup);
        }

        m_groupGroups.push_back(groupGroup);
        groupGroup->m_index = m_groupGroups.size() - 1;
        return groupGroup->m_index;
    }

    void updateAlpha();
    void updatePosition();
};