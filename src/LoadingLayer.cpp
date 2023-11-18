#include "LoadingLayer.hpp"

#include <string>
#include <array>
#include "SpriteFrameCache.hpp"
#include "Director.hpp"
#include "LevelLayer.hpp"
#include "TestLayer.hpp"
#include "utils.hpp"

#include <emscripten.h>

struct LoadSpriteArgs {
    SpriteFrameCache* cache;
    LoadingLayer* layer;
    std::string path;
    GLenum slot;
    float* percent;
    float increaseBy;
};

void* loadSpriteFrames(void* args) {
    auto [cache, layer, path, slot, percent, increaseBy] = *(LoadSpriteArgs*)args;
    cache->loadSpriteFramesFromPlist(path, slot, layer);

    layer->m_mutex.lock();
    *percent += increaseBy;
    layer->m_mutex.unlock();
    
    return NULL;
}

void* loadSprite(void* args) {
    auto [cache, layer, path, slot, percent, increaseBy] = *(LoadSpriteArgs*)args;
    cache->loadSprite(path, slot, layer);

    layer->m_mutex.lock();
    *percent += increaseBy;
    layer->m_mutex.unlock();

    return NULL;
}

void* loadResourcesThread(void* layerPtr) {
    auto layer = (LoadingLayer*)layerPtr;

    printf("Loading resources...\n");
    std::array<std::string, 4> gamesheets = {
        "static/gamesheets/GJ_GameSheet",
        "static/gamesheets/GJ_GameSheet02",
        "static/gamesheets/GJ_GameSheet03",
        "static/gamesheets/GJ_GameSheetGlow",
    };

    std::vector<THREAD_TYPE> threads;
    std::vector<std::shared_ptr<LoadSpriteArgs>> argHolder;

    for (auto& path : gamesheets) {
        THREAD_TYPE thread;
        auto args = std::make_shared<LoadSpriteArgs>(SpriteFrameCache::get(), layer, path, Texture::m_nextSlot++, &layer->m_percentDone, (0.75f / gamesheets.size()));
        THREAD_CREATE(thread, loadSpriteFrames, (void*)args.get());
        
        threads.push_back(thread);
        argHolder.push_back(args);
    }

    std::array<std::string, 47> sprites = { // dont make fun of me i got github copilot to do it for me
        "static/bgs/game_bg_01_001",
        "static/bgs/game_bg_02_001",
        "static/bgs/game_bg_03_001",
        "static/bgs/game_bg_04_001",
        "static/bgs/game_bg_05_001",
        "static/bgs/game_bg_06_001",
        "static/bgs/game_bg_07_001",
        "static/bgs/game_bg_08_001",
        "static/bgs/game_bg_09_001",
        "static/bgs/game_bg_10_001",
        "static/bgs/game_bg_11_001",
        "static/bgs/game_bg_12_001",
        "static/bgs/game_bg_13_001",
        "static/bgs/game_bg_14_001",
        "static/bgs/game_bg_15_001",
        "static/bgs/game_bg_16_001",
        "static/bgs/game_bg_17_001",
        "static/bgs/game_bg_18_001",
        "static/bgs/game_bg_19_001",
        "static/bgs/game_bg_20_001",
        "static/grounds/groundSquare_01_001",
        "static/grounds/groundSquare_02_001",
        "static/grounds/groundSquare_03_001",
        "static/grounds/groundSquare_04_001",
        "static/grounds/groundSquare_05_001",
        "static/grounds/groundSquare_06_001",
        "static/grounds/groundSquare_07_001",
        "static/grounds/groundSquare_08_001",
        "static/grounds/groundSquare_08_2_001",
        "static/grounds/groundSquare_09_001",
        "static/grounds/groundSquare_09_2_001",
        "static/grounds/groundSquare_10_001",
        "static/grounds/groundSquare_10_2_001",
        "static/grounds/groundSquare_11_001",
        "static/grounds/groundSquare_11_2_001",
        "static/grounds/groundSquare_12_001",
        "static/grounds/groundSquare_12_2_001",
        "static/grounds/groundSquare_13_001",
        "static/grounds/groundSquare_13_2_001",
        "static/grounds/groundSquare_14_001",
        "static/grounds/groundSquare_14_2_001",
        "static/grounds/groundSquare_15_001",
        "static/grounds/groundSquare_15_2_001",
        "static/grounds/groundSquare_16_001",
        "static/grounds/groundSquare_16_2_001",
        "static/grounds/groundSquare_17_001",
        "static/grounds/groundSquare_17_2_001",
    };

    for (auto& path : sprites) {
        THREAD_TYPE thread;
        auto args = std::make_shared<LoadSpriteArgs>(SpriteFrameCache::get(), layer, path, GL_TEXTURE0, &layer->m_percentDone, (0.25f / (sprites.size())));
        THREAD_CREATE(thread, loadSprite, (void*)args.get());

        threads.push_back(thread);
        argHolder.push_back(args);
    }

    for (auto& thread : threads) {
        THREAD_JOIN(thread);
    }
    
    layer->m_stage = LoadingStage::NONE;

    MAIN_THREAD_EM_ASM({
        Module.initialized();
    });

    return NULL;
}

LoadingLayer::LoadingLayer(bool loadResources) {
    if (loadResources) THREAD_CREATE(m_thread, loadResourcesThread, (void*)this);
}

void LoadingLayer::update(float delta) {
    m_mutex.lock();
    
    for (auto& uncommittedTexture : m_uncommittedTextures) {
        uncommittedTexture->m_texture = std::make_shared<Texture>(uncommittedTexture->imageData, Size {uncommittedTexture->width, uncommittedTexture->height}, GL_TEXTURE_2D, uncommittedTexture->slot, GL_RGBA, GL_UNSIGNED_BYTE);
        uncommittedTexture->m_finished = true;
    }

    m_uncommittedTextures.clear();

    for (auto& uncommittedDataTexture : m_uncommittedDataTextures) {
        uncommittedDataTexture->m_texture = std::make_shared<Texture>(uncommittedDataTexture->width, uncommittedDataTexture->height, uncommittedDataTexture->texType, uncommittedDataTexture->slot, uncommittedDataTexture->format, uncommittedDataTexture->pixelType);
        uncommittedDataTexture->m_texture->setUniforms(uncommittedDataTexture->uniform);
        uncommittedDataTexture->m_finished = true;
    }

    m_uncommittedDataTextures.clear();

    m_mutex.unlock();
}

void LoadingLayer::draw() {
    EM_ASM({
        Module.updateLoading($0, $1);
    }, m_percentDone, (int)m_stage);
};