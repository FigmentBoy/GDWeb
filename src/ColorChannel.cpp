#include "ColorChannel.hpp"

void ColorChannel::updateColor(float time) {
    if (!m_colorTriggers) {
        return;
    }

    RGBAColor oldColor = *m_currColor;
    ColorChannelValue res = m_colorTriggers->valueFor(time);
    
    setBlending(res.m_blending);

    RGBAColor state = (RGBAColor) res;
    if (oldColor != state) {
        *m_currColor = state;

        HSVAColor hsvaState = state.toHSVA();
        for (auto& channel : m_childChannels) {
            channel->parentUpdated(hsvaState);
        }

        for (auto& sprite : m_sprites) {
            sprite->m_dirtyColor = true;
        }
    }
}

void ColorChannel::setBlending(bool blending) {
    *m_blending = blending;
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

        *m_currColor = lbg;

        for (auto& sprite : m_sprites) {
            sprite->m_dirtyColor = true;
        }

        return;
    }

    newColor.h += m_inheritedDelta.h;
    newColor.s += m_inheritedDelta.s;
    newColor.v += m_inheritedDelta.v;
    *m_currColor = newColor.toRGBA();

    for (auto& sprite : m_sprites) {
        sprite->m_dirtyColor = true;
    }
}