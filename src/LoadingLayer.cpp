#include "LoadingLayer.hpp"

#include <string>
#include <array>
#include "SpriteFrameCache.hpp"
#include "Director.hpp"
#include "LevelLayer.hpp"
#include "TestLayer.hpp"
#include "utils.hpp"

#include "imgui.h"
#include "GUI.hpp"

void* loadResources(void* layerPtr) {
    auto layer = (LoadingLayer*)layerPtr;

    printf("Loading resources...\n");
    std::array<std::string, 4> gamesheets = {
        "static/gamesheets/GJ_GameSheet",
        "static/gamesheets/GJ_GameSheet02",
        "static/gamesheets/GJ_GameSheet03",
        "static/gamesheets/GJ_GameSheetGlow",
    };

    for (auto& path : gamesheets) {
        SpriteFrameCache::get()->loadSpriteFramesFromPlist(path, layer);
        layer->m_percentDone += (1.0f / gamesheets.size()) * 0.5f;
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
        "static/grounds/groundSquare_08_02_001",
        "static/grounds/groundSquare_09_001",
        "static/grounds/groundSquare_09_02_001",
        "static/grounds/groundSquare_10_001",
        "static/grounds/groundSquare_10_02_001",
        "static/grounds/groundSquare_11_001",
        "static/grounds/groundSquare_11_02_001",
        "static/grounds/groundSquare_12_001",
        "static/grounds/groundSquare_12_02_001",
        "static/grounds/groundSquare_13_001",
        "static/grounds/groundSquare_13_02_001",
        "static/grounds/groundSquare_14_001",
        "static/grounds/groundSquare_14_02_001",
        "static/grounds/groundSquare_15_001",
        "static/grounds/groundSquare_15_02_001",
        "static/grounds/groundSquare_16_001",
        "static/grounds/groundSquare_16_02_001",
        "static/grounds/groundSquare_17_001",
        "static/grounds/groundSquare_17_02_001",
    };

    for (auto& path : sprites) {
        SpriteFrameCache::get()->loadSprite(path, layer);
        layer->m_percentDone += (1.0f / (sprites.size() - 1)) * 0.5f;
    }
    
    layer->m_doneLoading = true;

    return NULL;
}

LoadingLayer::LoadingLayer() {
    THREAD_CREATE(m_thread, loadResources, (void*)this);
}

void LoadingLayer::update(float delta) {
    if (m_hasNewImage) {
        m_texture = new Texture(m_imageData, m_imageSize, GL_TEXTURE_2D, m_spriteSlot, GL_RGBA, GL_UNSIGNED_BYTE);
        printf("Texture added\n");
        m_hasNewImage = false;
    }

    if (m_doneLoading) {
        printf("Switching scenes...\n");
        Director::get()->swapRootNode(new LevelLayer(Level::fromGMD("static/Sonic Wave.gmd")));
        // Director::get()->swapRootNode(new TestLayer());
    }
}

void LoadingLayer::draw() {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoBackground;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;

    ImGui::SetNextWindowPos({0, 0});
    ImGui::SetNextWindowSize({ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y});
    
    ImGui::Begin("#LOADING", nullptr, window_flags);
    ImGui::PushFont(GUI::get()->micross->giant);

    ImVec2 size = ImGui::CalcTextSize(string_format("%.2f%% Loaded", m_percentDone * 100.0f).c_str());
    ImGui::SetCursorPos({ImGui::GetIO().DisplaySize.x / 2 - size.x / 2, ImGui::GetIO().DisplaySize.y / 2 - size.y / 2});
    ImGui::Text("%.2f%% Loaded", m_percentDone * 100.0f);
    
    ImGui::PopFont();
    ImGui::End();
};