#version 460 core

out vec4 FragColor;

in vec2 textureCoords;

uniform sampler2D baseTexture;

void main()
{
    FragColor = texture(baseTexture, textureCoords);
}
