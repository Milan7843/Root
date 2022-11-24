#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

layout(std430, binding = 0) buffer Tiles
{
	int textureIndex[];
};

out vec2 FragIn_TexCoords;

in int GeoIn_TileIndex[];
in int GeoIn_LayerIndex[];

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
		vec2 tileUV = offsets[i] + vec2(0.5, 0.5);
		//FragIn_TexCoords = (tileUV + textureIndex[GeoIn_TileIndex[0]]) / vec2(textureGridSize);
		FragIn_TexCoords = (tileUV + vec2(textureIndex[GeoIn_TileIndex[0] * 2], textureIndex[GeoIn_TileIndex[0] * 2 + 1])) / vec2(textureGridSize);

		vec4 basePosition = gl_in[0].gl_Position;
		basePosition.y = -basePosition.y;

		gl_Position = basePosition
			+ projection * view * model * vec4(offsets[i] * tileSize, 0.0, 1.0);

		// Depth
		gl_Position.z = renderDepth + 0.001 * GeoIn_LayerIndex[0];

		EmitVertex();
		if (i == 2 || i == 5)
		{
			EndPrimitive();
		}
	}
}
