#version 460 core

in vec2 FragIn_TexCoords;
out vec4 color;

uniform sampler2D sprite;

void main()
{
	color = texture(sprite, FragIn_TexCoords);
}