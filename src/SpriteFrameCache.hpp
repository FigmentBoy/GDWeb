#pragma once

#include <map>
#include <string>

#include "SpriteFrame.hpp"
#include "LoadingLayer.hpp"

class SpriteFrameCache {
public:    
    std::unordered_map<std::string, std::shared_ptr<SpriteFrame>> m_spriteFrames;

    static SpriteFrameCache* get() {
        static SpriteFrameCache* sharedCache = new SpriteFrameCache();
        return sharedCache;
    }

    void loadSprite(std::string path, LoadingLayer* loadingLayer=nullptr);
    void loadSpriteFramesFromPlist(std::string plistPath, LoadingLayer* loadingLayer=nullptr);
    
    std::shared_ptr<SpriteFrame> getSpriteFrameByName(std::string name);

};