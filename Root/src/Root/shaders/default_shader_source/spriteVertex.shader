#version 460 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

uniform int columnCount;
uniform int rowCount;
uniform int columnIndex;
uniform int rowIndex;

uniform vec2 offset;

uniform float renderDepth;

void main()
{
    // Screen position
    gl_Position = projection * view * model * vec4(position + offset, 0.0, 1.0);
    // Depth
    gl_Position.z = renderDepth;

    if (columnCount == 0 || rowCount == 0)
    {
        TexCoords = uv;
    }
    else
    {
        float spriteWidth = 1.0 / float(columnCount);
        float spriteHeight = 1.0 / float(rowCount);

        TexCoords = vec2(
            (float(columnIndex) / float(columnCount)) + (uv.x * spriteWidth),
            (float(rowIndex) / float(rowCount)) + (uv.y * spriteHeight)
        );
    }
}