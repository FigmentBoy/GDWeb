#version 300 es
precision highp float;

in vec2 texCoord;
in float color;
in float groupGroup;
in float texIndex;

out vec4 fragColor;

uniform sampler2D textures[6];

uniform sampler2D colorTexture;
uniform sampler2D groupGroupTexture;

void main() {
    vec4 channelColor = texelFetch(colorTexture, ivec2(0, color), 0);
    bool additiveBlending = texelFetch(colorTexture, ivec2(1, color), 0).r > 0.5;

    channelColor.a *= texelFetch(groupGroupTexture, ivec2(0, groupGroup), 0).r;

    fragColor = channelColor;
    switch(int(texIndex)) { // I don't want to make a Sampler2DArray :)
        case 0:
            fragColor = texture(textures[0], texCoord) * channelColor;
            break;
        case 1:
            fragColor = texture(textures[1], texCoord) * channelColor;
            break;
        case 2:
            fragColor = texture(textures[2], texCoord) * channelColor;
            break;
        case 3:
            fragColor = texture(textures[3], texCoord) * channelColor;
            break;
        case 4:
            fragColor = texture(textures[4], texCoord) * channelColor;
            break;
        case 5:
            fragColor = texture(textures[5], texCoord) * channelColor;
            break;
    }

    fragColor.rgb *= fragColor.a;
    if (additiveBlending) fragColor.a = 0.0;
}