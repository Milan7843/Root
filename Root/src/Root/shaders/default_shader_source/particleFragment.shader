#version 460 core

in vec2 FragIn_TexCoords;
in vec3 FragIn_BaseColor;
out vec4 color;

uniform bool useTexture;
uniform sampler2D sprite;

void main()
{
    if (useTexture)
    {
        color = texture(sprite, FragIn_TexCoords) * vec4(FragIn_BaseColor, 1.);
        return;
    }

    color = vec4(FragIn_BaseColor, 1.);
}