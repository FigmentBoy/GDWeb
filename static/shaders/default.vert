#version 330

in vec3 vertPosition;
in vec2 aTex;
in vec4 aColor;
in float aTexIndex;
in float aBlending;

out vec2 texCoord;
out vec4 color;
out float texIndex;
out float blending;

uniform mat4 ProjectionMatrix;

void main() {
    texCoord = aTex;
    color = aColor;
    texIndex = aTexIndex;
    blending = aBlending;

    gl_Position = ProjectionMatrix * vec4(vertPosition, 1.0);
}