#include "ColorChannel.hpp"
#include "ChannelTriggers.hpp"

void ColorChannel::updateTextureColor() {
    m_textureColor = m_currColor;

    m_colorTexture->setSubData(
        &m_currColor.r, 
        {1, 1}, {0, m_index}, GL_FLOAT
    );
}

void ColorChannel::updateTextureBlending() {
    m_textureBlending = m_blending;

    GLfloat data[4] = {
        m_blending ? 1.0f : 0.0f,
        0.0f,
        0.0f,
        0.0f
    };

    m_colorTexture->setSubData(
        data, 
        {1, 1}, {1, m_index}, GL_FLOAT
    );
}

ColorChannelValue ColorChannel::valueFor(float time) {
    ColorChannelValue res = valueForWithoutPulses(time);

    if (m_pulseTriggers && m_pulseTriggers->m_changes.lower_bound(time) != m_pulseTriggers->m_changes.begin()) {
        res = {m_pulseTriggers->valueFor(time), res.m_blending};
    }

    return res;
}

ColorChannelValue ColorChannel::valueForWithoutPulses(float time) {
    ColorChannelValue res = {m_colorCopied && m_parentChannel ? m_parentChannel->valueFor(time).shift(m_inheritedDelta) : m_baseColor, m_baseBlending};
    if (m_colorCopied && !m_copyOpacity) res.a = m_baseColor.a;
    
    if (m_colorTriggers && m_colorTriggers->m_changes.lower_bound(time) != m_colorTriggers->m_changes.begin()) {
        res = m_colorTriggers->valueFor(time);
    }

    return res;
}

void ColorChannel::updateColor(float time) {
    if (m_index == 1007) { // LBG
        RGBAColor bg = m_parentChannel->valueFor(time);
        HSVAColor newColor = bg.toHSVA();

        newColor.s -= 0.2;
        newColor.v += 0.2;
        RGBAColor lbg = newColor.toRGBA();

        float f = (bg.r + bg.g + bg.b) * 255.f / 150.f;

        if (f < 1.f) {
            lbg.r = lbg.r * f + m_p1Color.r * (1 - f);
            lbg.g = lbg.g * f + m_p1Color.g * (1 - f);
            lbg.b = lbg.b * f + m_p1Color.b * (1 - f);
        }

        m_currColor = lbg;
        updateTextureColor();

        return;
    }

    auto res = valueFor(time);

    m_currColor = res;
    m_blending = res.m_blending;

    if (m_blending != m_textureBlending) {
        updateTextureBlending();
    }

    if (m_currColor != m_textureColor) {
        updateTextureColor();
        
        HSVAColor hsvaState = m_currColor.toHSVA();
        for (auto& channel : m_childChannels) {
            channel->updateColor(time);
        }
    }
}
