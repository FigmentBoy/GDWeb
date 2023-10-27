#version 300 es
precision highp float;

in vec2 texCoord;
in vec4 color;
in float texIndex;
in float blending;

out vec4 fragColor;

uniform sampler2D textures[6];

void main() {
    bool additiveBlending = blending > 0.5;

    fragColor = vec4(0);
    switch(int(texIndex)) { // I don't want to make a Sampler2DArray :)
        case 0:
            fragColor = texture(textures[0], texCoord) * color;
            break;
        case 1:
            fragColor = texture(textures[1], texCoord) * color;
            break;
        case 2:
            fragColor = texture(textures[2], texCoord) * color;
            break;
        case 3:
            fragColor = texture(textures[3], texCoord) * color;
            break;
        case 4:
            fragColor = texture(textures[4], texCoord) * color;
            break;
        case 5:
            fragColor = texture(textures[5], texCoord) * color;
            break;
    }

    fragColor.rgb *= fragColor.a;
    if (additiveBlending) fragColor.a = 0.0;
}