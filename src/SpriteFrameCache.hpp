#pragma once

#include <map>
#include <string>

#include "SpriteFrame.hpp"
#include "LoadingLayer.hpp"

class SpriteFrameCache {
public:
    std::mutex m_mutex;
    std::unordered_map<std::string, std::shared_ptr<SpriteFrame>> m_spriteFrames;

    static SpriteFrameCache* get() {
        static SpriteFrameCache* sharedCache = new SpriteFrameCache();
        return sharedCache;
    }

    void loadSprite(std::string path, GLenum slot, LoadingLayer* loadingLayer=nullptr);
    void loadSpriteFramesFromPlist(std::string plistPath, GLenum slot, LoadingLayer* loadingLayer=nullptr);
    
    std::shared_ptr<SpriteFrame> getSpriteFrameByName(std::string name);

};