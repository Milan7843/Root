#pragma once

#include <Root/Logger.h>
#include <Root/Time.h>

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define NO_RULE '~'
#define EXISTS '+'
#define NOT_EXISTS '-'

enum class MultipleTextureIndexUsage
{
	ANIMATION,
	RANDOM_OF
};

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

struct Tile
{
	char tag;

	MultipleTextureIndexUsage multipleTextureIndexUsage;
	std::vector<glm::ivec2> textureIndices;
	unsigned int textureIndex;

	// A '~' means that there is no rule set for that neighbouring grid space.
	// A '+' means that the rule is satisfied if a tile exists there.
	// A '-' means that the rule is satisfied if no tile exists there.
	// A letter means that the rule is satisfied if a tile exists there,
	// with a tag matching the letter.
	std::string rules;

	// Whether to offset each tile individually to a random texture index in the animation
	bool randomIndexOffset;

	float animationSpeed;
	float timeSinceAnimationChange{ 0.0f };
	unsigned int textureIndicesStartIndex{ 0 }; // What index to start the texture indexing at
};

struct ShaderTile
{
	int baseIndex;
	int tileTextureIndicesStartIndex;
	int tileTextureIndexCount;
	bool randomTileIndexOffset;
};

class TileSet
{
public:

	~TileSet();

	/*
	static void create(const std::string& path,
		const std::string& name);
	*/

	static void create(std::vector<Tile>& tiles,
		const std::string& name,
		float animationSpeed = 1.0f);

	static Tile createTile(char tag,
		std::vector<glm::ivec2> textureIndices,
		char topLeftRule,
		char topMiddleRule,
		char topRightRule,
		char middleLeftRule,
		char middleRightRule,
		char bottomLeftRule,
		char bottomMiddleRule,
		char bottomRightRule,
		unsigned int textureStartIndex = 0,
		MultipleTextureIndexUsage multipleTextureIndexUsage = MultipleTextureIndexUsage::ANIMATION,
		float animationSpeed = 1.0f,
		bool randomAnimationOffset = false);

	static Tile createTile(char tag,
		glm::ivec2 textureIndex,
		char topLeftRule,
		char topMiddleRule,
		char topRightRule,
		char middleLeftRule,
		char middleRightRule,
		char bottomLeftRule,
		char bottomMiddleRule,
		char bottomRightRule);

	void update();

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

	/**
	 * Bind the shader buffer.
	 */
	void bindSSBO();

private:

	TileSet(std::vector<Tile>& tiles,
		float animationSpeed);

	//static Tile readTile(std::ifstream& file);

	void verifyTileStartIndices();

	void generateTileIndicesSSBO();

	void generateTileInformationSSBO();

	unsigned int tilesSSBO{ 0 };
	unsigned int tileTextureIndicesSSBO{ 0 };
	
	float animationSpeed{ 1.0f };

	std::vector<Tile> tiles;
};

