#include "LevelLoadingLayer.hpp"
#include "LevelLayer.hpp"
#include "Level.hpp"
#include "Director.hpp"

#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
void loadLevel(char* string) {
    std::string levelString = string;
    free(string);

    Director::get()->swapRootNode(new LevelLoadingLayer(new Level(levelString)));    
}

void* loadLevelThread(void* ptr) {
    auto layer = (LevelLoadingLayer*)ptr;

    layer->m_stage = LoadingStage::LEVEL;

    layer->m_levelLayer = new LevelLayer(layer->m_level, layer);
    layer->m_done = true;

    return NULL;
}

LevelLoadingLayer::LevelLoadingLayer(Level* level) : m_level(level), LoadingLayer(false) {
    EM_ASM({
        if (Module.audio) Module.audio.pause();
        Module.exitedLevel();
    });

    LevelLayer::m_instance = nullptr;

    m_stage = LoadingStage::LEVEL;
    m_percentDone = 0.0f;

    THREAD_CREATE(m_thread, loadLevelThread, (void*)this);
}

void LevelLoadingLayer::update(float delta) {
    if (m_done) {
        Director::get()->swapRootNode(m_levelLayer);

        m_stage = LoadingStage::NONE;
        m_percentDone = 1.0f;

        EM_ASM({
            Module.updateLoading($0, $1);
            Module.enteredLevel();
        }, m_percentDone, (int)m_stage);
    
        return;
    }

    LoadingLayer::update(delta);
}