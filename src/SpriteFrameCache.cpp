#include "SpriteFrameCache.hpp"
#include "Texture.hpp"

#include "Plist.hpp"
#include "utils.hpp"

#include "PlistSpriteParser.hpp"

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
    // TODO: Abstract the following chunk of code into a function. Duplicate code with the code in SpriteFrameCache::loadSprite
    std::shared_ptr<Texture> texture;
    if (loadingLayer) {
        Texture::getImageData((path + "-uhd.png").c_str(), loadingLayer);
        loadingLayer->m_spriteSlot = m_spriteSlot;
        loadingLayer->m_hasNewImage = true;
        while (loadingLayer->m_hasNewImage) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        };
        texture = std::shared_ptr<Texture>(loadingLayer->m_texture);
    } else {
        texture = std::make_shared<Texture>((path + "-uhd.png").c_str(), GL_TEXTURE_2D, m_spriteSlot, GL_RGBA, GL_UNSIGNED_BYTE);
    }

    std::cout << texture->m_id << std::endl;
    std::cout << texture->m_slot << " SUPPOSED TO BE " << m_spriteSlot << std::endl;
    // TODO: End

    PlistSpriteParser parser;

    if (!parser.parse(path + "-uhd.plist", texture, this))
        return;
    
    m_spriteSlot++;
}

void SpriteFrameCache::addSpriteFrame(std::string name, std::shared_ptr<SpriteFrame> spriteFrame) {
    m_spriteFrames[name] = spriteFrame;
}

std::shared_ptr<SpriteFrame> SpriteFrameCache::getSpriteFrameByName(std::string name) {
    if (m_spriteFrames.find(name) == m_spriteFrames.end()) {
        std::cout << "SpriteFrameCache::getSpriteFrameByName: Invalid spriteframe name " + name << std::endl;
        return nullptr;
    }

    return m_spriteFrames[name];
}