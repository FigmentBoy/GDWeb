#pragma once

#include "SpriteFrameCache.hpp"
#include "Texture.hpp"

#include <Plist.hpp>

#include <string>

class PlistSpriteParser {
public:
    PlistSpriteParser();

    bool parse(std::string path, std::shared_ptr<Texture> texture, SpriteFrameCache* cache);

private:
    std::vector<int> bracketStringToIntArray(boost::any string);

    Point bracketStringToPoint(boost::any string);

    Rect bracketStringToRect(boost::any string);

    std::shared_ptr<SpriteFrame> parseSpriteFrame(std::map<std::string, boost::any> frame);

private:
    std::shared_ptr<Texture> m_texture;

};