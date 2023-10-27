#pragma once

#include "Triggers.hpp"
#include "Color.hpp"
#include "Sprite.hpp"
#include <map>

struct ColorTrigger {
    float duration;
    RGBAColor toColor;
};

struct AlphaTrigger {
    float duration;
    float toAlpha;
};

// TODO:
// struct PulseTrigger {
//     float time;
//     float period;
// };

class ColorChannel {
public:
    std::shared_ptr<RGBAColor> m_currColor;
    std::shared_ptr<bool> m_blending;

    RGBAColor m_baseColor;

    bool m_playerColor;
    int m_index;
    
    bool m_colorCopied;
    HSVAColor m_inheritedDelta = {0, 0, 0};

    inline static RGBAColor m_p1Color = {1, 0, 1};
    inline static RGBAColor m_p2Color = {0, 1, 0.5};

    std::vector<std::shared_ptr<ColorChannel>> m_childChannels;
    std::unique_ptr<GameEffect<ColorChange>> m_colorTriggers;

    ColorChannel() : m_currColor(std::make_shared<RGBAColor>(1.0f, 1.0f, 1.0f, 1.0f)), m_baseColor({1.0f, 1.0f, 1.0f, 1.0f}), m_blending(std::make_shared<bool>(false)) {};

    virtual void parentUpdated(HSVAColor parentColor);
    void setBlending(bool blending);
    void updateColor(float time);
};