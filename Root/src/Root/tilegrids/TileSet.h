#pragma once

#include <Root/Logger.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

enum class RulePosition {
	TOP_RIGHT = 0,
	MIDDLE_RIGHT = 1,
	BOTTOM_RIGHT = 2,
	BOTTOM_MIDDLE = 3,
	BOTTOM_LEFT = 4,
	MIDDLE_LEFT = 5,
	TOP_LEFT = 6,
	TOP_MIDDLE = 7
};

struct Tile {
	char tag;
	std::vector<glm::ivec2> textureIndices;
	// A '~' means that there is no rule set for that neighbouring grid space.
	// A '+' means that the rule is satisfied if a tile exists there.
	// A '-' means that the rule is satisfied if no tile exists there.
	// A letter means that the rule is satisfied if a tile exists there,
	// with a tag matching the letter.
	std::string rules;
};

class TileSet
{
public:

	~TileSet();

	static void create(const std::string& path,
		const std::string& name);

	static void create(std::vector<Tile>& tiles,
		const std::string& name);

	/**
	 * Set a tile rule on a specific tile
	 */
	void setTileRule(unsigned int tileIndex,
		RulePosition rulePosition,
		char rule);

	/**
	 * Get the tiles.
	 * 
	 * \return all tiles in this tile set.
	 */
	std::vector<Tile>& getTiles();

private:

	TileSet(std::vector<Tile>& tiles);

	static Tile readTile(std::ifstream& file);


	std::vector<Tile> tiles;
};

