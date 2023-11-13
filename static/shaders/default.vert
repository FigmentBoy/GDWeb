#version 300 es
precision highp float;

in vec3 vertPosition;
in vec2 aTex;
in float aColor;
in float aGroupGroup;
in float aTexIndex;

out vec2 texCoord;
out float color;
out float groupGroup;
out float texIndex;

uniform mat4 ProjectionMatrix;

uniform sampler2D groupGroupTexture;

void main() {
    texCoord = aTex;
    color = aColor;
    groupGroup = aGroupGroup;
    texIndex = aTexIndex;

    gl_Position = ProjectionMatrix * vec4(vertPosition, 1.0);
}