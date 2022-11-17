#version 460 core

layout(location = 0) in vec2 uv;

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
}