#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 size;
layout(location = 3) in float rotation;

out vec3 baseColor;
out vec2 particleSize;
out float particleRotation;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform float renderDepth;

void main()
{
    // Screen position
    gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
    // Depth
    gl_Position.z = renderDepth;

    baseColor = color;

    particleSize = size;

    particleRotation = rotation;
}