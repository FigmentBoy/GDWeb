#pragma once

#include "Color.hpp"
#include "Sprite.hpp"
#include "GameEffect.hpp"
#include "Batcher.hpp"
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

class ColorChannelValue;
using ColorChange = TypeChanger<ColorChannelValue>;

class PulseValue;
using PulseChange = CompoundTypeChanger<PulseValue>;

class ColorChannel {
public:
    std::shared_ptr<Texture> m_colorTexture;

    RGBAColor m_textureColor;
    RGBAColor m_currColor;
    bool m_blending;

    RGBAColor m_baseColor;

    bool m_playerColor;
    int m_index;
    
    bool m_colorCopied = false;
    HSVAColor m_inheritedDelta = {0, 1, 1};
    float percentCopiage = 1.0f;

    inline static RGBAColor m_p1Color = {1, 0, 1};
    inline static RGBAColor m_p2Color = {0, 1, 0.5};

    std::shared_ptr<ColorChannel> m_parentChannel;
    std::vector<std::shared_ptr<ColorChannel>> m_childChannels;

    std::unique_ptr<GameEffect<ColorChange>> m_colorTriggers;
    std::unique_ptr<GameEffect<PulseChange>> m_pulseTriggers;

    std::shared_ptr<Batcher> m_batcher;

    ColorChannel() : m_currColor(1.0f, 1.0f, 1.0f, 1.0f), m_baseColor(1.0f, 1.0f, 1.0f, 1.0f), m_blending(false) {};

    void setBlending(bool blending);
    void updateColor(float time);

    ColorChannelValue valueFor(float time);
    ColorChannelValue valueForWithoutPulses(float time);

    void updateTextureColor();
    void updateTextureBlending();
};