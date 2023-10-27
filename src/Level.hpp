#pragma once

#include <string>

class Level {
public:
    std::string m_levelName;
    std::string m_levelString;
    int64_t m_audioTrack;

    static Level* fromGMD(std::string path);
    static Level* fromServers(int id);

    void decodeLevelString();
};