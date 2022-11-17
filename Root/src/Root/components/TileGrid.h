#pragma once

#include <Root/components/Component.h>
#include <Root/Transform.h>

#include <vector>
#include <string>
#include <memory>

class TileGrid : public Component
{
public:

	~TileGrid();

	/**
	 * Create a new tile grid.
	 * 
	 * \param transform: the transform to add this tile grid to.
	 * \param texture: the index of the texture to use.
	 * \param gridSize: the size of the grid (y rows by x columns).
	 * \param data: the path to the tile grid data.
	 */
	static TileGrid* create(Transform* transform,
		unsigned int texture,
		glm::ivec2 gridSize,
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

	/**
	 * Set a tile rule on a specific tile
	 */
	void setTileRule(unsigned int tileIndex,
		RulePosition rulePosition,
		TileRule rule);

private:

	TileGrid(unsigned int texture,
		glm::ivec2 gridSize,
		const std::string& dataPath);

	void readData(const std::string& dataPath);

	glm::ivec2 gridSize;
	float tileSize{ 1.0f };

	std::vector<unsigned int> tileIndices;
	std::vector<Tile> tiles;

	unsigned int textureID{ 0 };
	unsigned int tileMapVAO{ 0 };
};
