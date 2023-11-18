#version 300 es
precision highp float;

in vec3 vertPosition;
in vec2 aTex;
in float aColor;
in float aGroupGroup;
in float aTexIndex;
in vec4 aHSV;
in float aBlending;

out vec2 texCoord;
out float color;
out float groupGroup;
out float texIndex;
out vec4 hsv;
out float blending;

uniform mat4 ProjectionMatrix;

uniform sampler2D groupGroupTexture;

void main() {
    texCoord = aTex;
    color = aColor;
    groupGroup = aGroupGroup;
    texIndex = aTexIndex;
    hsv = aHSV;
    blending = aBlending;

    vec3 groupGroupPosition = vec3(
        texelFetch(groupGroupTexture, ivec2(1, groupGroup), 0).xy,
        0.0
    );

    gl_Position = ProjectionMatrix * vec4(vertPosition + groupGroupPosition, 1.0);
}