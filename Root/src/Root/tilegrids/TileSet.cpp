#include "TileSet.h"

#include <Root/engine/TileGridEngine.h>

TileSet::~TileSet()
{
}

/*
void TileSet::create(const std::string& path, const std::string& name)
{
	// Reading in the file
	std::ifstream file(path);
	if (!file.is_open())
	{
		Logger::logError("Cannot read tile set from '" + path + "'; File cannot be opened.");
		return;
	}

	std::vector<Tile> tiles;


	// ====== ANIMATION SPEED READING ====== //

	// Holds a full line
	std::string line;
	float animationSpeed = 0.0f;

	// Stop at the end or on an error
	while (file.good())
	{
		// Find the next char
		char c = file.get();

		// Just go the the next line
		if (c == '\n')
		{
			continue;
		}

		// Skip entire line after #
		if (c == '#')
		{
			std::getline(file, line);
			continue;
		}

		// Check whether the character is a valid number
		if (TileGridEngine::isNumber(c))
		{
			animationSpeed = TileGridEngine::readFloat(&file, c);

			if (animationSpeed <= 0.0f)
			{
				Logger::logError("Tile grid animation speed cannot be less than or equal to 0; was " + std::to_string(animationSpeed));
				return;
			}

			break;
		}
	}

	// Keep reading tiles
	while (true)
	{
		try
		{
			// Stop if the file is done
			if (!file.good())
				break;

			// Reading a new tile
			Tile tile = readTile(file);
			tiles.push_back(tile);
		}
		catch (const char* err)
		{
			if (err != "End of file reached.")
			{
				Logger::logError(err);
				return;
			}
			break;
		}
	}

	// Creating a tile set from the tiles
	TileSet* tileSet = new TileSet(tiles, animationSpeed);
	std::shared_ptr<TileSet> pointer = std::shared_ptr<TileSet>(tileSet);

	TileGridEngine::addTileSet(pointer, name);
}
*/

void TileSet::create(std::vector<Tile>& tiles,
	const std::string& name,
	float animationSpeed)
{
	if (animationSpeed <= 0.0f)
	{
		Logger::logError("Tile grid animation speed cannot be less than or equal to 0; was " + std::to_string(animationSpeed));
		return;
	}

	// Creating a tile set from the tiles
	TileSet* tileSet = new TileSet(tiles, animationSpeed);
	std::shared_ptr<TileSet> pointer = std::shared_ptr<TileSet>(tileSet);

	TileGridEngine::addTileSet(pointer, name);
}

Tile TileSet::createTile(char tag,
	std::vector<glm::ivec2> textureIndices,
	char topLeftRule,
	char topMiddleRule,
	char topRightRule,
	char middleLeftRule,
	char middleRightRule,
	char bottomLeftRule,
	char bottomMiddleRule,
	char bottomRightRule,
	unsigned int textureStartIndex,
	MultipleTextureIndexUsage multipleTextureIndexUsage,
	float animationSpeed,
	bool randomAnimationOffset)
{
	std::string rules{ "        " };
	rules[0] = topLeftRule;
	rules[1] = topMiddleRule;
	rules[2] = topRightRule;
	rules[3] = middleLeftRule;
	rules[4] = middleRightRule;
	rules[5] = bottomLeftRule;
	rules[6] = bottomMiddleRule;
	rules[7] = bottomRightRule;

	return Tile{ tag,
		multipleTextureIndexUsage,
		textureIndices,
		textureStartIndex,
		rules,
		randomAnimationOffset || multipleTextureIndexUsage == MultipleTextureIndexUsage::RANDOM_OF,
		animationSpeed
	};
}

Tile TileSet::createTile(char tag,
	glm::ivec2 textureIndex,
	char topLeftRule,
	char topMiddleRule,
	char topRightRule,
	char middleLeftRule,
	char middleRightRule,
	char bottomLeftRule,
	char bottomMiddleRule,
	char bottomRightRule)
{
	std::string rules{ "        " };
	rules[0] = topLeftRule;
	rules[1] = topMiddleRule;
	rules[2] = topRightRule;
	rules[3] = middleLeftRule;
	rules[4] = middleRightRule;
	rules[5] = bottomLeftRule;
	rules[6] = bottomMiddleRule;
	rules[7] = bottomRightRule;

	return Tile{ tag,
		MultipleTextureIndexUsage::ANIMATION,
		std::vector<glm::ivec2>{ textureIndex },
		0,
		rules,
		false,
		1.0f
	};
}

void TileSet::update()
{
	bool anyTileUpdated{ false };

	// Updating every tile
	for (Tile& tile : tiles) {
		// Must have at least 2 texture indices to animate
		if (tile.textureIndices.size() <= 1) {
			continue;
		}

		if (tile.multipleTextureIndexUsage == MultipleTextureIndexUsage::ANIMATION)
		{
			// Check if an update is timely
			if (tile.timeSinceAnimationChange > 1.0f / tile.animationSpeed)
			{
				// Moving to the next texture index
				if (tile.textureIndex >= tile.textureIndices.size() - 1)
					tile.textureIndex = 0;
				else
					tile.textureIndex++;

				// At least one tile was updated
				anyTileUpdated = true;

				// Reset update timer
				tile.timeSinceAnimationChange = 0.0f;
			}

			tile.timeSinceAnimationChange += Time::getDeltaTime() * animationSpeed;
		}
	}

	// Updating the SSBO
	if (anyTileUpdated)
		generateTileInformationSSBO();
}

/*
Tile TileSet::readTile(std::ifstream& file)
{
	// Properties
	char tag{ '-' };
	std::vector<glm::ivec2> textureCoordinates;

	bool enableDebugLogging{ true };


	const std::string allowedTags{ "abcdefghijklmnopqrstuvwxyz" };

	// Holds a full line
	std::string line;

	unsigned int lineNumber{ 0 };


	// ====== TAG READING ====== //

	// Stop at the end or on an error
	while (file.good())
	{
		// Find the next char
		char c = file.get();

		// Just go the the next line
		if (c == '\n')
		{
			lineNumber++;
			continue;
		}

		// Skip entire line after #
		if (c == '#')
		{
			std::getline(file, line);
			lineNumber++;
			continue;
		}

		// Check whether the character is a valid tag
		if (allowedTags.find(c) != std::string::npos)
		{
			if (enableDebugLogging)
				Logger::log(std::string("Found tag: ") + c);

			tag = c;

			break;
		}
	}

	if (tag == '-')
	{
		throw "End of file reached.";
	}


	// ====== TEXTURE COORDINATES READING ====== //

	glm::ivec2 currentCoordinate{ glm::ivec2(0) };

	bool doneReadingCoordinates{ false };

	bool foundCoordinateInfoThisLine{ true };

	// Stop at the end or on an error
	while (!doneReadingCoordinates) // Keep reading coordinates
	{
		bool readFirstCoordinate{ false };

		while (file.good())
		{
			// Find the next char
			char c = file.get();

			// Just go the the next line or continue on certain characters
			if (c == '\n')
			{
				lineNumber++;
				if (!foundCoordinateInfoThisLine && textureCoordinates.size() != 0)
				{
					doneReadingCoordinates = true;
					break;
				}

				foundCoordinateInfoThisLine = false;

				continue;
			}
			// Just continue on certain characters
			if (c == ',' || c == '(' || c == ')' || c == ' ')
			{
				continue;
			}

			// Skip entire line after #
			if (c == '#')
			{
				std::getline(file, line);
				lineNumber++;

				if (!foundCoordinateInfoThisLine && textureCoordinates.size() != 0)
				{
					doneReadingCoordinates = true;
					break;
				}

				continue;
			}

			// We must now be at a coordinate number
			Logger::log(std::string("Attempting to get number at ") + c);
			unsigned int coordinate{ TileGridEngine::readNumber(&file, c) };

			if (!readFirstCoordinate)
			{
				if (enableDebugLogging)
					Logger::log("Found first coordinate: " + std::to_string(coordinate));

				currentCoordinate.x = coordinate;
				readFirstCoordinate = true;
			}
			else
			{
				if (enableDebugLogging)
					Logger::log("Found second coordinate: " + std::to_string(coordinate));

				currentCoordinate.y = coordinate;

				// Finished reading a full coordinate
				textureCoordinates.push_back(currentCoordinate);
				break;
			}

			foundCoordinateInfoThisLine = true;
		}

		if (!file.good())
		{
			doneReadingCoordinates = true;
		}
	}

	if (!file.good())
	{
		throw "End of file reached before finding tile rules.";
	}


	// ====== TILE RULES READING ====== //
	unsigned int currentTileRuleIndex{ 0 }; // 0 to 7

	std::string rules{ "        " };

	while (file.good())
	{
		// Find the next char
		char c = file.get();

		// Just go the the next line or continue on space
		if (c == '\n')
		{
			lineNumber++;
			continue;
		}
		if (c == ' ')
		{
			continue;
		}

		// Skip entire line after #
		if (c == '#')
		{
			std::getline(file, line);
			lineNumber++;
			continue;
		}

		// Reading the tile rule now
		// The rule must be one of the allowed values or a tag
		if (c == '~' || c == '-' || c == '+' || allowedTags.find(c) != std::string::npos)
		{
			if (enableDebugLogging)
				Logger::log(std::string("Found rule: ") + c);
			rules[currentTileRuleIndex] = c;

			if (currentTileRuleIndex == 7)
			{
				break;
			}

			currentTileRuleIndex++;
		}
		else
		{
			continue;
		}
	}

	if (currentTileRuleIndex != 7)
	{
		// Something must have gone wrong
		throw "Did not find all tile rules.";
	}

	return Tile{ tag, textureCoordinates, 0, rules };
}
*/

void TileSet::generateTileIndicesSSBO()
{
	// Create a new buffer if none exists
	if (tileTextureIndicesSSBO == 0)
		glGenBuffers(1, &tileTextureIndicesSSBO);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, tileTextureIndicesSSBO);

	/*
	* Main idea: have a list of possible texture indices per tile.
	* Each Grid cell will have an offset of 0-(n-1), which will be added to the base index,
	* and wrapped if it gets too big.
	* This way there is control over each tile at once by the base index, and control over tiles separately
	* by the offset.
	*/

	std::vector<glm::ivec2> tileTextureIndices;

	// Adding each texture index of each tile to the buffer
	unsigned int index{ 0 };
	for (Tile& tile : tiles)
	{
		for (glm::ivec2& textureIndex : tile.textureIndices)
		{
			tileTextureIndices.push_back(glm::ivec2(textureIndex));

			index++;
		}
	}

	// Loading the UV data into the new buffer
	glBufferData(GL_SHADER_STORAGE_BUFFER, tileTextureIndices.size() * sizeof(glm::ivec2), tileTextureIndices.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tileTextureIndicesSSBO);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TileSet::generateTileInformationSSBO()
{
	// Create a new buffer if none exists
	if (tilesSSBO == 0)
		glGenBuffers(1, &tilesSSBO);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, tilesSSBO);

	/*
	* Main idea: have a list of possible texture indices per tile.
	* Each Grid cell will have an offset of 0-(n-1), which will be added to the base index,
	* and wrapped if it gets too big.
	* This way there is control over each tile at once by the base index, and control over tiles separately
	* by the offset.
	*/

	std::vector<ShaderTile> shaderTiles(tiles.size());

	// Adding each texture index of each tile to the buffer
	unsigned int index{ 0 };
	for (unsigned int i = 0; i < tiles.size(); i++)
	{
		Tile& tile = tiles[i];

		shaderTiles[i].baseIndex = tile.textureIndex;

		shaderTiles[i].tileTextureIndicesStartIndex = index;

		// The number of different tile indices it has
		shaderTiles[i].tileTextureIndexCount = tile.textureIndices.size();

		// Whether to use random index offsets
		shaderTiles[i].randomTileIndexOffset = tile.randomIndexOffset;

		index += tile.textureIndices.size();
	}

	// Loading the UV data into the new buffer
	glBufferData(GL_SHADER_STORAGE_BUFFER, shaderTiles.size() * sizeof(ShaderTile), shaderTiles.data(), GL_STATIC_DRAW);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, tilesSSBO);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void TileSet::setTileRule(unsigned int tileIndex, RulePosition rulePosition, char rule)
{
}

std::vector<Tile>& TileSet::getTiles()
{
	return tiles;
}

void TileSet::bindSSBO()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, tilesSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, tilesSSBO);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, tileTextureIndicesSSBO);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, tileTextureIndicesSSBO);
}

TileSet::TileSet(std::vector<Tile>& tiles,
	float animationSpeed)
	: tiles(tiles)
	, animationSpeed(animationSpeed)
{
	verifyTileStartIndices();

	// Generating all required SSBOs
	generateTileIndicesSSBO();
	generateTileInformationSSBO();
}

void TileSet::verifyTileStartIndices()
{
	unsigned int cumulativeIndex{ 0 };
	for (Tile& tile : tiles)
	{
		tile.textureIndicesStartIndex = cumulativeIndex;
		cumulativeIndex += tile.textureIndices.size();
	}
}
