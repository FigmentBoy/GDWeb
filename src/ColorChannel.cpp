#include "ColorChannel.hpp"
#include "ChannelTriggers.hpp"

void ColorChannel::updateTextureColor() {
    m_textureColor = m_currColor;

    GLubyte data[4] = {
        static_cast<GLubyte>(m_currColor.r * 255), 
        static_cast<GLubyte>(m_currColor.g * 255), 
        static_cast<GLubyte>(m_currColor.b * 255), 
        static_cast<GLubyte>(m_currColor.a * 255),
    };
    m_colorTexture->setSubData(
        data, 
        {1, 1}, {0, m_index}, GL_UNSIGNED_BYTE
    );
}

void ColorChannel::updateTextureBlending() {
    m_textureBlending = m_blending;

    GLubyte data[4] = {
        static_cast<GLubyte>(m_blending ? 255 : 0),
        0,
        0,
        0
    };
    m_colorTexture->setSubData(
        data, 
        {1, 1}, {1, m_index}, GL_UNSIGNED_BYTE
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
    ColorChannelValue res = {m_colorCopied && m_parentChannel ? m_parentChannel->valueFor(time).shift(m_inheritedDelta) : m_baseColor, m_blending};

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

    m_currColor = m_colorCopied && m_parentChannel ? m_parentChannel->valueFor(time).shift(m_inheritedDelta) : m_baseColor;
    
    auto ret = valueFor(time);

    m_blending = ret.m_blending;
    m_currColor = ret;

    if (m_blending != m_textureBlending) { // Re-sort
        updateTextureBlending();
        m_batcher->m_dirty = true;
    }

    if (m_currColor != m_textureColor) {
        updateTextureColor();
        
        HSVAColor hsvaState = m_currColor.toHSVA();
        for (auto& channel : m_childChannels) {
            channel->updateColor(time);
        }
    }
}
