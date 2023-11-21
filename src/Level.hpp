#pragma once

#include <string>

class Level {
public:
    std::string m_levelString;

    static Level* fromGMD(std::string path);

    Level() {};
    Level(std::string levelString) : m_levelString(levelString) {
        decodeLevelString();
    };

    void decodeLevelString();
};