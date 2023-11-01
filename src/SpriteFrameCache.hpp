#pragma once

#include <map>
#include <string>

#include "SpriteFrame.hpp"
#include "LoadingLayer.hpp"

class SpriteFrameCache {
public:
    inline static GLenum m_spriteSlot = GL_TEXTURE0;
    
    std::unordered_map<std::string, std::shared_ptr<SpriteFrame>> m_spriteFrames;

    static SpriteFrameCache* get() {
        static SpriteFrameCache* sharedCache = new SpriteFrameCache();
        return sharedCache;
    }

    void loadSprite(std::string path, LoadingLayer* loadingLayer=nullptr);
    void loadSpriteFramesFromPlist(std::string plistPath, LoadingLayer* loadingLayer=nullptr);

    void addSpriteFrame(std::string name, std::shared_ptr<SpriteFrame> spriteFrame);
    
    std::shared_ptr<SpriteFrame> getSpriteFrameByName(std::string name);

};