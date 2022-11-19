#pragma once

#include <Root/components/Component.h>
#include <Root/Transform.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

enum class GridSpaceType
{
	EMPTY,
	INDEX,
	TAG
};

struct GridSpace
{
	GridSpaceType type;
	unsigned int index; // For index grid spaces
	char tag; // For tag grid spaces
};

class TileGrid : public Component
{
public:

	~TileGrid();

	/**
	 * Create a new tile grid.
	 * 
	 * \param transform: the transform to add this tile grid to.
	 * \param texture: the index of the texture to use.
	 * \param data: the path to the tile grid data.
	 */
	static TileGrid* create(Transform* transform,
		unsigned int texture,
		const std::string& dataPath);

	void render(float renderDepth) override;

	/**
	 * Add a tile to the tile grid.
	 * 
	 * \param textureIndex: the 2-dimensional index of the texture in the texture atlas.
	 * 
	 * \returns: the index to reference this tile by.
	 */
	unsigned int addTile(glm::ivec2 textureIndex);

	void setTileTextureIndex(unsigned int tileIndex, glm::ivec2 textureIndex);

private:

	TileGrid(unsigned int texture,
		const std::string& dataPath);

	void readData(const std::string& dataPath);

	float tileSize{ 1.0f };

	std::string tileSet{ "" };

	std::vector<unsigned int> tileIndices;

	glm::ivec2 tileGridSize{ glm::ivec2(0) };

	unsigned int layerCount{ 0 };
	
	GridSpace* gridSpaces;

	unsigned int textureID{ 0 };
	unsigned int tileMapVAO{ 0 };
};
