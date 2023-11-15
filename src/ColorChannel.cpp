#include "ColorChannel.hpp"
#include "Triggers.hpp"

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

RGBAColor ColorChannel::valueFor(float time) {
    if (!m_colorTriggers) return m_currColor;
    return m_colorTriggers->valueFor(time);
}

void ColorChannel::updateColor(float time) {
    if (!m_colorTriggers || (m_colorCopied && time < m_colorTriggers->m_changes.begin()->first)) {
        return;
    }

    RGBAColor oldColor = m_currColor;
    ColorChannelValue res = m_colorTriggers->valueFor(time);
    
    auto oldBlending = m_blending;
    m_blending = res.m_blending;
    if (oldBlending != m_blending) { // Re-sort
        updateTextureBlending();
        m_batcher->m_dirty = true;
    }

    RGBAColor state = (RGBAColor) res;
    if (oldColor != state) {
        m_currColor = state;
        updateTextureColor();

        HSVAColor hsvaState = state.toHSVA();
        for (auto& channel : m_childChannels) {
            channel->parentUpdated(hsvaState, time);
        }
    }
}

void ColorChannel::updatePulse(float time) {
    if (!m_pulseTriggers) return;

    PulseValue res = m_pulseTriggers->valueFor(time);

    auto old = m_currColor;
    m_currColor = PulseChange::apply(m_currColor, res);
    if (m_currColor != old) {
        updateTextureColor();
        m_currColor = old;

        HSVAColor hsvaState = m_currColor.toHSVA();
        for (auto& channel : m_childChannels) {
            channel->parentUpdated(hsvaState, time);
        }
    }
}



void ColorChannel::parentUpdated(HSVAColor parentColor, float time) {
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

    if (m_colorTriggers && time >= m_colorTriggers->m_changes.begin()->first) return;

    newColor.h += m_inheritedDelta.h;
    newColor.h = fmod(newColor.h, 360.0f);

    if (m_inheritedDelta.addS) newColor.s += m_inheritedDelta.s;
    else newColor.s *= m_inheritedDelta.s;

    newColor.s = std::clamp(newColor.s, 0.0f, 1.0f);

    if (m_inheritedDelta.addV) newColor.v += m_inheritedDelta.v;
    else newColor.v *= m_inheritedDelta.v;

    newColor.v = std::clamp(newColor.v, 0.0f, 1.0f);

    for (auto& channel : m_childChannels) {
        channel->parentUpdated(newColor, time);
    }
    
    m_currColor = newColor.toRGBA();
    updateTextureColor();
}