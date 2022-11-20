#pragma once

#include <Root/tilegrids/TileSet.h>

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
	FILL_IN,
	INDEX,
	TAG,
	OUT_OF_BOUNDS
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
	 * \param texture: the path to the texture to use.
	 * \param pixelPerfect: whether the texture should be pixel perfect.
	 * \param data: the path to the tile grid data.
	 * \param tileSetName: the name of the tile set to use.
	 */
	static TileGrid* create(Transform* transform,
		const std::string& texturePath,
		bool pixelPerfect,
		const std::string& dataPath,
		const std::string& tileSetName);

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

	/**
	 * Set the size for each single tile.
	 * 
	 * \param tileSize: the new tile size.
	 */
	void setTileSize(float tileSize);

private:

	TileGrid(unsigned int texture,
		GridSpace* data,
		glm::ivec2 tileGridSize,
		unsigned int layerCount,
		std::string tileSet);

	static TileGrid* readData(const std::string& texturePath,
		bool pixelPerfect,
		const std::string& dataPath,
		const std::string& tileSetName);

	void generateTileIndices();

	GridSpace getGridSpaceAtIndex(unsigned int x, unsigned int y, unsigned int layer);

	void autoFillGridSpace(TileSet* tileSet,
		unsigned int x,
		unsigned int y,
		unsigned int layer,
		char requiredTag);

	float tileSize{ 1.0f };

	std::string tileSetName{ "" };

	int* tileIndices{ nullptr };

	glm::ivec2 tileGridSize{ glm::ivec2(0) };

	unsigned int layerCount{ 0 };
	
	GridSpace* gridSpaces;

	unsigned int textureID{ 0 };
	unsigned int tileMapVAO{ 0 };
};
