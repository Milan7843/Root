#pragma once

#include <Root/Logger.h>

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

enum class RulePosition {
	TOP_RIGHT,
	MIDDLE_RIGHT,
	BOTTOM_RIGHT,
	BOTTOM_MIDDLE,
	BOTTOM_LEFT,
	MIDDLE_LEFT,
	TOP_LEFT,
	TOP_MIDDLE
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

private:

	TileSet(std::vector<Tile>& tiles);

	static Tile readTile(std::ifstream& file);


	std::vector<Tile> tiles;
};

