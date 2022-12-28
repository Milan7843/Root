#version 460 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

out vec2 textureCoords;

void main()
{
	gl_Position = vec4(position, -1., 1.);

	textureCoords = uv;
}