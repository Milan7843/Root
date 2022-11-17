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

enum class TileRule {
	PRESENCE,
	ABSENCE,
	NONE
};

struct Tile {
	glm::ivec2 textureIndex;
	TileRule rules[8];
};

class TileSet
{
public:

	~TileSet();

	static void create(const std::string& path,
		const std::string& name);

private:

	TileSet();

	std::vector<Tile> tiles;
};

