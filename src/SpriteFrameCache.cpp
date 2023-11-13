#include "SpriteFrameCache.hpp"
#include "Texture.hpp"

#include "Plist.hpp"
#include "utils.hpp"

#include <thread>

void SpriteFrameCache::loadSprite(std::string path, LoadingLayer* loadingLayer) {
    std::shared_ptr<Texture> texture;

    if (loadingLayer) {
        Texture::getImageData((path + "-uhd.png").c_str(), loadingLayer);
        loadingLayer->m_spriteSlot = GL_TEXTURE0;
        loadingLayer->m_hasNewImage = true;
        while (loadingLayer->m_hasNewImage) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        };
        texture = std::shared_ptr<Texture>(loadingLayer->m_texture);
    } else {
        texture = std::make_shared<Texture>((path + "-uhd.png").c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    }

    std::cout << texture->m_id << std::endl;
    std::cout << texture->m_slot << " SUPPOSED TO BE " << GL_TEXTURE0 << std::endl;
    
    std::shared_ptr<SpriteFrame> spriteFrame = std::make_shared<SpriteFrame>(texture, texture->m_size);

    std::cout << path.substr(path.rfind("/") + 1) + ".png" << std::endl;

    if (path.rfind("/") != std::string::npos)
        m_spriteFrames[path.substr(path.rfind("/") + 1) + ".png"] = spriteFrame;
    else
        m_spriteFrames[path + ".png"] = spriteFrame;
}

void SpriteFrameCache::loadSpriteFramesFromPlist(std::string path, LoadingLayer* loadingLayer) {
    std::shared_ptr<Texture> texture;
    if (loadingLayer) {
        Texture::getImageData((path + "-uhd.png").c_str(), loadingLayer);
        loadingLayer->m_spriteSlot = Texture::m_nextSlot;
        loadingLayer->m_hasNewImage = true;
        while (loadingLayer->m_hasNewImage) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        };
        texture = std::shared_ptr<Texture>(loadingLayer->m_texture);
    } else {
        texture = std::make_shared<Texture>((path + "-uhd.png").c_str(), GL_TEXTURE_2D, Texture::m_nextSlot, GL_RGBA, GL_UNSIGNED_BYTE);
    }

    std::cout << texture->m_id << std::endl;
    std::cout << texture->m_slot << " SUPPOSED TO BE " << Texture::m_nextSlot << std::endl;

    std::map<std::string, boost::any> dict;
    Plist::readPlist((path + "-uhd.plist").c_str(), dict);

    if (dict.find("frames") == dict.end()) {
        std::cout << "SpriteFrameCache::loadSpriteFramesFromPlist: Invalid spriteframe plist" << std::endl;
        return;
    }

    std::map<std::string, boost::any> frames = boost::any_cast<std::map<std::string, boost::any>>(dict["frames"]);
    
    for (const auto& frame : frames) {
        std::string frameName = frame.first;
        
        if (frame.second.type() != typeid(std::map<std::string, boost::any>))
            continue;
        
        std::map<std::string, boost::any> frameDict = boost::any_cast<std::map<std::string, boost::any>>(frame.second);
        std::shared_ptr<SpriteFrame> spriteFrame = std::make_shared<SpriteFrame>();

        std::string textureRect = boost::any_cast<std::string>(frameDict["textureRect"]);
        
        textureRect.erase(std::remove(textureRect.begin(), textureRect.end(), '{'), textureRect.end());
        textureRect.erase(std::remove(textureRect.begin(), textureRect.end(), '}'), textureRect.end());

        std::vector<std::string> splitRect = split(textureRect, ",");
        
        spriteFrame->m_texCoords.position.x = std::stoi(splitRect[0]);
        spriteFrame->m_texCoords.position.y = std::stoi(splitRect[1]);
        spriteFrame->m_texCoords.size.width = std::stoi(splitRect[2]);
        spriteFrame->m_texCoords.size.height = std::stoi(splitRect[3]);

        spriteFrame->m_texCoords = spriteFrame->m_texCoords / texture->m_size;

        spriteFrame->m_size.width = std::stoi(splitRect[2]);
        spriteFrame->m_size.height = std::stoi(splitRect[3]);

        std::string spriteOffset = boost::any_cast<std::string>(frameDict["spriteOffset"]);

        spriteOffset.erase(std::remove(spriteOffset.begin(), spriteOffset.end(), '{'), spriteOffset.end());
        spriteOffset.erase(std::remove(spriteOffset.begin(), spriteOffset.end(), '}'), spriteOffset.end());

        std::vector<std::string> splitOffset = split(spriteOffset, ",");

        spriteFrame->m_offset.x = std::stoi(splitOffset[0]);
        spriteFrame->m_offset.y = std::stoi(splitOffset[1]);
        
        spriteFrame->m_rotated = boost::any_cast<bool>(frameDict["textureRotated"]);

        if (spriteFrame->m_rotated) {
            std::swap(spriteFrame->m_texCoords.size.width, spriteFrame->m_texCoords.size.height);
        }

        spriteFrame->m_texture = texture;

        spriteFrame->m_size /= 4.f;
        spriteFrame->m_offset /= 4.f;

        m_spriteFrames[frameName] = spriteFrame;
    }
    std::cout << "Added " + std::to_string(frames.size()) + " sprites from " + path << std::endl;
    Texture::m_nextSlot++;
}

std::shared_ptr<SpriteFrame> SpriteFrameCache::getSpriteFrameByName(std::string name) {
    if (m_spriteFrames.find(name) == m_spriteFrames.end()) {
        std::cout << "SpriteFrameCache::getSpriteFrameByName: Invalid spriteframe name " + name << std::endl;
        return nullptr;
    }

    return m_spriteFrames[name];
}