#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

layout(std140, binding = 0) buffer Tiles
{
	ivec2 textureIndex[];
};

out vec2 FragIn_TexCoords;

in int GeoIn_TileIndex[];

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float renderDepth;

uniform float tileSize;

uniform ivec2 textureGridSize;

vec2 offsets[] = {
	vec2( 0.5, 0.5),
	vec2( 0.5,-0.5),
	vec2(-0.5, 0.5),
	vec2( 0.5,-0.5),
	vec2(-0.5,-0.5),
	vec2(-0.5, 0.5)
};

void main()
{
	// Empty tile
	if (GeoIn_TileIndex[0] == -1)
	{
		return;
	}

	for (int i = 0; i < 6; i++)
	{
		FragIn_TexCoords = (offsets[i] + vec2(0.5, 0.5) + textureIndex[GeoIn_TileIndex[0]]) / vec2(textureGridSize);
		FragIn_TexCoords = offsets[i] + vec2(0.5, 0.5);

		gl_Position = gl_in[0].gl_Position
			+ projection * view * model * vec4(offsets[i] * tileSize, 0.0, 1.0);

		// Depth
		gl_Position.z = renderDepth;

		EmitVertex();
		if (i == 2 || i == 5)
		{
			EndPrimitive();
		}
	}
}
