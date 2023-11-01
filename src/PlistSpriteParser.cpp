#include "PlistSpriteParser.hpp"
#include "utils.hpp"

#include <utility>

PlistSpriteParser::PlistSpriteParser() {}

bool PlistSpriteParser::parse(std::string path, std::shared_ptr<Texture> texture, SpriteFrameCache* cache) {
    m_texture = texture;

    std::map<std::string, boost::any> dict;
    Plist::readPlist(path.c_str(), dict);

    if (dict.find("frames") == dict.end()) {
        std::cout << "PlistSpriteParser::parse: Invalid spriteframe plist" << std::endl;
        return false;
    }

    std::map<std::string, boost::any> frames = boost::any_cast<std::map<std::string, boost::any>>(dict["frames"]);

    for (const auto& frame : frames) {
        std::string frameName = frame.first;
        if (frame.second.type() != typeid(std::map<std::string, boost::any>))
            continue;
        
        std::map<std::string, boost::any> frameDict = boost::any_cast<std::map<std::string, boost::any>>(frame.second);
        cache->addSpriteFrame(frameName, parseSpriteFrame(frameDict));
    }

    std::cout << "Added " + std::to_string(frames.size()) + " sprites from " + path << std::endl;
    return true;
}

std::string removeCharacters(std::string string, char character) {
    string.erase(std::remove(string.begin(), string.end(), character), string.end());
    return string;
}

std::vector<int> PlistSpriteParser::bracketStringToIntArray(boost::any bstring) {
    std::string string = boost::any_cast<std::string>(bstring);

    string = removeCharacters(string, '{');
    string = removeCharacters(string, '}');

    std::vector<std::string> splitStr = split(string, ",");
    std::vector<int> res;

    for (const auto& str : splitStr) {
        res.push_back(std::stoi(str));
    }

    return res;
}
    
Point PlistSpriteParser::bracketStringToPoint(boost::any string) {
    auto array = bracketStringToIntArray(string);

    return Point(array[0], array[1]);
}

Rect PlistSpriteParser::bracketStringToRect(boost::any string) {
    auto array = bracketStringToIntArray(string);

    return Rect(array[0], array[1], array[2], array[3]);
}

std::shared_ptr<SpriteFrame> PlistSpriteParser::parseSpriteFrame(std::map<std::string, boost::any> frame) {
    std::shared_ptr<SpriteFrame> spriteFrame = std::make_shared<SpriteFrame>();

    spriteFrame->m_texCoords = bracketStringToRect(frame["textureRect"]);
    spriteFrame->m_offset    = bracketStringToPoint(frame["spriteOffset"]);
    spriteFrame->m_rotated   = boost::any_cast<bool>(frame["textureRotated"]);

    spriteFrame->m_size = spriteFrame->m_texCoords.size;
    spriteFrame->m_texCoords = spriteFrame->m_texCoords / m_texture->m_size;
    
    if (spriteFrame->m_rotated) {
        std::swap(spriteFrame->m_texCoords.size.width, spriteFrame->m_texCoords.size.height);
    }

    spriteFrame->m_texture = m_texture;

    spriteFrame->m_size /= 4.f;
    spriteFrame->m_offset /= 4.f;

    return spriteFrame;
}