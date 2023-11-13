#include "ColorChannel.hpp"

void ColorChannel::updateTextureColor() {
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

void ColorChannel::updateColor(float time) {
    if (!m_colorTriggers) {
        return;
    }

    RGBAColor oldColor = m_currColor;
    ColorChannelValue res = m_colorTriggers->valueFor(time);
    
    auto oldBlending = m_blending;
    m_blending = res.m_blending;
    if (oldBlending != m_blending) updateTextureBlending();

    RGBAColor state = (RGBAColor) res;
    if (oldColor != state) {
        m_currColor = state;
        updateTextureColor();

        HSVAColor hsvaState = state.toHSVA();
        for (auto& channel : m_childChannels) {
            channel->parentUpdated(hsvaState);
        }
    }
}



void ColorChannel::parentUpdated(HSVAColor parentColor) {
    HSVAColor newColor = parentColor;

    if (m_index == 1007) { // LBG
        RGBAColor bg = parentColor.toRGBA();
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

    newColor.h += m_inheritedDelta.h;
    newColor.s += m_inheritedDelta.s;
    newColor.v += m_inheritedDelta.v;
    m_currColor = newColor.toRGBA();

    updateTextureColor();
}