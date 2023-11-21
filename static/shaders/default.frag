#version 300 es
precision highp float;

in vec2 texCoord;
in float color;
in float groupGroup;
in float texIndex;
in vec4 hsv;
in float blending;
in float spriteType;

out vec4 fragColor;

uniform sampler2D textures[6];

uniform sampler2D colorTexture;
uniform sampler2D groupGroupTexture;

uniform int[1] visibilities;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 shiftHSV(vec4 color, float h, float s, float v, bool addS, bool addV) {
    float alpha = color.w;

    vec3 hsv = rgb2hsv(color.xyz);

    hsv.x += h;
    hsv.x = mod(hsv.x + 1.0, 1.0);

    if (addS) {
        hsv.y += s;
        hsv.y = clamp(hsv.y, 0.0, 1.0);
    } else {
        hsv.y *= s;
    }

    if (addV) {
        hsv.z += v;
        hsv.z = clamp(hsv.z, 0.0, 1.0);
    } else {
        hsv.z *= v;
    }

    return vec4(hsv2rgb(hsv), alpha);
}

void main() {
    if (spriteType >= 0.0 && visibilities[int(spriteType)] == 0) {
        discard;
    }
    
    float blendingVal = texelFetch(colorTexture, ivec2(1, color), 0).x;
    bool additiveBlending = blendingVal > 0.5;

    if (blending < 1.5) {
        if (blending > 0.5 != additiveBlending) {
            discard;
        }
    } else {
        additiveBlending = false;
    }

    fragColor = vec4(0.0);
    switch(int(texIndex)) { // I don't want to make a Sampler2DArray :)
        case 0:
            fragColor = texture(textures[0], texCoord);
            break;
        case 1:
            fragColor = texture(textures[1], texCoord);
            break;
        case 2:
            fragColor = texture(textures[2], texCoord);
            break;
        case 3:
            fragColor = texture(textures[3], texCoord);
            break;
        case 4:
            fragColor = texture(textures[4], texCoord);
            break;
        case 5:
            fragColor = texture(textures[5], texCoord);
            break;
    }

    vec4 channelColor = texelFetch(colorTexture, ivec2(0, abs(color)), 0);

    if (color < 0.0) {
        channelColor = shiftHSV(channelColor, 0.0, 0.0, -20.0, true, true);
    }

    channelColor.a *= texelFetch(groupGroupTexture, ivec2(0, groupGroup), 0).r;

    if (hsv.w != 0.0) {
        bool sChecked = hsv.w == 1.5 || hsv.w == 2.0;
        bool vChecked = hsv.w == 0.5 || hsv.w == 1.5;

        channelColor = shiftHSV(channelColor, hsv.x, hsv.y, hsv.z, sChecked, vChecked);
    }

    if (additiveBlending) {
        fragColor *= channelColor;
        fragColor.rgb *= pow(fragColor.a, 2.0);
        fragColor.a = 0.0;
    } else {
        fragColor *= channelColor;
        fragColor.rgb *= fragColor.a;
    }
}