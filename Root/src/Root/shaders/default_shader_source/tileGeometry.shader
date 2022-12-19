#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

struct Tile
{
	int baseIndex;
	int tileTextureIndicesStartIndex;
	int tileTextureIndexCount;
	bool randomTileIndexOffset;
};

layout(std430, binding = 0) buffer Tiles
{
	Tile tiles[];
};

layout(std430, binding = 1) buffer TileTextureIndices
{
	ivec2 textureIndex[];
};

out vec2 FragIn_TexCoords;

in int GeoIn_TileIndex[];
in int GeoIn_LayerIndex[];
in int GeoIn_TileID[];

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform float renderDepth;

uniform float tileSize;

uniform int tileIndexRandomisationSeed;

uniform ivec2 textureGridSize;

vec2 offsets[] = {
	vec2( 0.5, 0.5),
	vec2( 0.5,-0.5),
	vec2(-0.5, 0.5),
	vec2( 0.5,-0.5),
	vec2(-0.5,-0.5),
	vec2(-0.5, 0.5)
};

float rand(int seed)
{
	return sin(mod(float(seed * 14), 6.14)) * 10000.;
}

int randInt(int seed)
{
	return abs(int(rand(seed)));
}

void main()
{
	// Empty tile
	if (GeoIn_TileIndex[0] == -1)
	{
		return;
	}

	int finalTextureIndex = tiles[GeoIn_TileIndex[0]].baseIndex;

	// Adding some random offset to the index
	if (tiles[GeoIn_TileIndex[0]].randomTileIndexOffset)
	{
		finalTextureIndex += randInt(tileIndexRandomisationSeed + GeoIn_TileID[0]);
	}

	// Wrapping the index
	finalTextureIndex = int(mod(finalTextureIndex, tiles[GeoIn_TileIndex[0]].tileTextureIndexCount));

	finalTextureIndex += tiles[GeoIn_TileIndex[0]].tileTextureIndicesStartIndex;

	for (int i = 0; i < 6; i++)
	{
		vec2 tileUV = offsets[i] + vec2(0.5, 0.5);
		//FragIn_TexCoords = (tileUV + textureIndex[GeoIn_TileIndex[0]]) / vec2(textureGridSize);
		FragIn_TexCoords = (tileUV + vec2(textureIndex[finalTextureIndex])) / vec2(textureGridSize);

		vec4 basePosition = gl_in[0].gl_Position;
		basePosition.y = -basePosition.y;

		gl_Position = basePosition
			+ projection * view * model * vec4(offsets[i] * tileSize, 0.0, 1.0);

		// Depth
		gl_Position.z = renderDepth + 0.00001 * GeoIn_LayerIndex[0];

		EmitVertex();
		if (i == 2 || i == 5)
		{
			EndPrimitive();
		}
	}
}
