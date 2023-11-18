#pragma once

#include <string>

class Level {
public:
    std::string m_levelName;
    std::string m_levelString;
    int m_audioTrack;

    static Level* fromGMD(std::string path);

    Level() {};
    Level(std::string levelName, std::string levelString, int audioTrack) : m_levelName(levelName), m_levelString(levelString), m_audioTrack(audioTrack) {
        decodeLevelString();
    };

    void decodeLevelString();
};