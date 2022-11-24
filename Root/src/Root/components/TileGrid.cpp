#include "TileGrid.h"

#include <Root/engine/TextureEngine.h>
#include <Root/engine/TileGridEngine.h>

TileGrid::~TileGrid()
{
	delete gridSpaces;

	if (tileIndices != nullptr)
		free(tileIndices);
}

TileGrid* TileGrid::create(Transform* transform,
	const std::string& texturePath,
	bool pixelPerfect,
	glm::ivec2 textureGridSize,
	const std::string& dataPath,
	const std::string& tileSetName)
{
	TileGrid* tileGrid = readData(texturePath, pixelPerfect, textureGridSize, dataPath, tileSetName);

	if (tileGrid == nullptr)
	{
		// Something went wrong: don't create a tile grid
		return nullptr;
	}

	std::shared_ptr<TileGrid> pointer{ tileGrid };
	transform->addComponent(pointer);
	return tileGrid;
}

void TileGrid::render(float renderDepth)
{
	// Retrieving the tile set
	TileSet* tileSet = TileGridEngine::getTileSet(tileSetName);

	if (tileSet == nullptr)
		return;

	tileSet->bindSSBO();

	// Getting the shader
	Shader* spriteRenderShader{ Root::getTileGridRenderShader() };

	// Setting shader variables
	spriteRenderShader->use();
	spriteRenderShader->setMat4("model", transform->getModelMatrix());
	spriteRenderShader->setMat4("view", Root::getActiveCamera()->getTransform()->getInverseTransformMatrix());

	spriteRenderShader->setMat4("projection", Root::getActiveCamera()->getProjectionMatrix());
	spriteRenderShader->setInt("sprite", 0);
	spriteRenderShader->setFloat("renderDepth", renderDepth / 10000.0f);
	spriteRenderShader->setIVector2("tileGridSize", tileGridSize);
	spriteRenderShader->setIVector2("textureGridSize", textureGridSize);
	spriteRenderShader->setFloat("tileSize", tileSize);

	// Binding the sprite
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glBindVertexArray(tileMapVAO);

	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_POINTS, 0, tileGridSize.x * tileGridSize.y * layerCount);

	glBindVertexArray(0);
}

unsigned int TileGrid::addTile(glm::ivec2 textureIndex)
{
	return 0;
}

void TileGrid::setTileTextureIndex(unsigned int tileIndex, glm::ivec2 textureIndex)
{
}

void TileGrid::setTileSize(float tileSize)
{
	this->tileSize = tileSize;
}

TileGrid::TileGrid(unsigned int texture,
	glm::ivec2 textureGridSize,
	GridSpace* data,
	glm::ivec2 tileGridSize,
	unsigned int layerCount,
	std::string tileSet)
	: textureID(texture)
	, textureGridSize(textureGridSize)
	, gridSpaces(data)
	, tileGridSize(tileGridSize)
	, layerCount(layerCount)
	, tileSetName(tileSet)
{
	std::cout << "HEREHRHEHREHRHER1" << std::endl;
	generateTileIndices();
}

TileGrid* TileGrid::readData(const std::string& texturePath,
	bool pixelPerfect,
	glm::ivec2 textureGridSize,
	const std::string& dataPath,
	const std::string& tileSetName)
{
	bool enableDebugLogging{ true };

	// Additional tile map data
	glm::ivec2 tileGridSize{ glm::ivec2(0) };
	unsigned int layerCount{ 0 };

	// Reading in the file
	std::ifstream file(dataPath);
	if (!file.is_open())
	{
		Logger::logError("Cannot read tile grid from '" + dataPath + "'; File cannot be opened.");
		return nullptr;
	}

	// Holds a full line
	std::string line;

	unsigned int lineNumber{ 0 };

	// Reading the tile grid dimensions
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

		// Finding the first number
		if (!TileGridEngine::isNumber(c))
		{
			continue;
		}

		// Reading the tile grid dimensions
		tileGridSize.y = TileGridEngine::readNumber(&file, c);
		file.get();
		c = file.get();
		tileGridSize.x = TileGridEngine::readNumber(&file, c);
		if (enableDebugLogging)
			Logger::log(std::string("Found tile grid size: ") + std::to_string(tileGridSize.x) + " x " + std::to_string(tileGridSize.y));
		break;
	}

	if (tileGridSize.x <= 0 || tileGridSize.y <= 0)
	{
		Logger::logError("Tile grid size must not be less than 1 in each dimension.");
		return nullptr;
	}

	// Reading the number of layers
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

		// Finding the first number
		if (!TileGridEngine::isNumber(c))
		{
			continue;
		}

		// Reading the layer count
		layerCount = TileGridEngine::readNumber(&file, c);
		if (enableDebugLogging)
			Logger::log(std::string("Found layer count: ") + std::to_string(layerCount));
		break;
	}

	if (layerCount <= 0)
	{
		Logger::logError("Tile layer count must not be less than 1.");
		return nullptr;
	}

	GridSpace* gridSpaces = (GridSpace*)malloc(tileGridSize.x * tileGridSize.y * layerCount * sizeof(GridSpace));

	const std::string allowedTags{ "abcdefghijklmnopqrstuvwxyz" };

	unsigned int currentRowIndex{ 0 };
	unsigned int currentLayerIndex{ 0 };

	// Reading the number of layers
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

		// Reached a data line
		for (unsigned int i{ 0 }; i < tileGridSize.x; i++)
		{
			// Find the next char except at the beginning
			if (i != 0)
				c = file.get();

			GridSpace gridSpace{};

			// Read an index
			if (TileGridEngine::isNumber(c))
			{
				gridSpace.type = GridSpaceType::INDEX;
				gridSpace.index = TileGridEngine::readNumber(&file, c);
				if (enableDebugLogging)
					Logger::log(std::string("Read grid space with index: ") + std::to_string(gridSpace.index));
			}
			else
			{
				// Read a char (tag or empty)
				if (c == ' ' || c == '-')
				{
					// Empty
					gridSpace.type = GridSpaceType::EMPTY;
					if (enableDebugLogging)
						Logger::log("Read empty grid space");
				}
				else if (c == '+')
				{
					// Fill-in
					gridSpace.type = GridSpaceType::FILL_IN;
					if (enableDebugLogging)
						Logger::log("Read fill-in grid space");
				}
				else
				{
					// Tagged
					if (allowedTags.find(c) == std::string::npos)
					{
						// Disallowed tag: empty space
						gridSpace.type = GridSpaceType::EMPTY;

						Logger::logError(std::string("Invalid tag found in layer ")
							+ std::to_string(currentLayerIndex + 1)
							+ " in column " + std::to_string(i + 1)
							+ " in row " + std::to_string(currentRowIndex + 1)
							+ ": " + c);
					}
					else
					{
						gridSpace.type = GridSpaceType::TAG;
						gridSpace.tag = c;
						if (enableDebugLogging)
							Logger::log(std::string("Read grid space with tag: ") + gridSpace.tag);
					}
				}
			}

			// Tile space index
			unsigned int index = 
				tileGridSize.x * tileGridSize.y * currentLayerIndex + 
				currentRowIndex * tileGridSize.x + 
				i;

			gridSpaces[index] = gridSpace;

			// Skip a space
			file.get();
		}
		currentRowIndex++;

		// Move to the next layer
		if (currentRowIndex >= tileGridSize.y)
		{
			currentRowIndex = 0;
			currentLayerIndex++;

			// Check if we read all layers
			if (currentLayerIndex >= layerCount)
			{
				break;
			}
		}
	}

	for (int i = 0; i < tileGridSize.x * tileGridSize.y * layerCount; i++)
	{
		switch (gridSpaces[i].type)
		{
			case GridSpaceType::EMPTY:
				std::cout << ' ' << ' ' << std::endl;
				break;
			case GridSpaceType::FILL_IN:
				std::cout << '+' << ' ' << std::endl;
				break;
			case GridSpaceType::TAG:
				std::cout << gridSpaces[i].tag << ' ' << std::endl;
				break;
			case GridSpaceType::INDEX:
				std::cout << gridSpaces[i].index << ' ' << std::endl;
				break;
		}
	}

	// Loading the texture
	unsigned int textureID = TextureEngine::loadTexture(texturePath, pixelPerfect);

	TileGrid* tileGrid = new TileGrid(textureID, textureGridSize, gridSpaces, tileGridSize, layerCount, tileSetName);
	return tileGrid;
}

GridSpace TileGrid::getGridSpaceAtIndex(unsigned int x, unsigned int y, unsigned int layer)
{
	// Check for out of local bounds
	if (x < 0 || x >= tileGridSize.x ||
		y < 0 || y >= tileGridSize.y)
	{
		return GridSpace{ GridSpaceType::OUT_OF_BOUNDS };
	}

	// Grid space index
	unsigned int index =
		tileGridSize.x * tileGridSize.y * layer +
		y * tileGridSize.x +
		x;

	/*
	if (index >= tileGridSize.x * tileGridSize.y * layerCount)
		Logger::logError("Getting grid space data at index out of bounds.");
	*/

	return gridSpaces[index];
}

void TileGrid::autoFillGridSpace(TileSet* tileSet,
	unsigned int x,
	unsigned int y,
	unsigned int layer,
	char requiredTag)
{
	// Check for out of local bounds
	if (x < 0 || x >= tileGridSize.x ||
		y < 0 || y >= tileGridSize.y ||
		layer < 0 || layer >= layerCount)
	{
		return;
	}

	// Grid space index
	unsigned int index =
		tileGridSize.x * tileGridSize.y * layer +
		y * tileGridSize.x +
		x;

	glm::ivec2 offsets[] = {
		glm::ivec2(-1, -1),
		glm::ivec2(0, -1),
		glm::ivec2(1, -1),
		glm::ivec2(-1, 0),
		glm::ivec2(1, 0),
		glm::ivec2(-1, 1),
		glm::ivec2(0, 1),
		glm::ivec2(1, 1),
	};

	bool tileFound{ false };

	unsigned int tileIndex{ 0 };

	for (Tile& tile : tileSet->getTiles())
	{
		// If the required tag is not '-', we must check if it matches the tile
		if (requiredTag != '-' && tile.tag != requiredTag)
		{
			// Skip if it doesn't match
			continue;
		}

		bool ruleBroken{ false };

		// Checking all rules for this tile:
		for (unsigned int i{ 0 }; i < 8; i++)
		{
			char rule = tile.rules[i];

			GridSpace ruleSpace = getGridSpaceAtIndex(x + offsets[i].x, y + offsets[i].y, layer);

			// A '~' means that there is no rule set for that neighbouring grid space.
			if (rule == '~')
			{
				// Rule is always true
				continue;
			}

			// A '+' means that the rule is satisfied if a tile exists there.
			if (rule == '+')
			{
				// Verify the rule
				if (ruleSpace.type != GridSpaceType::EMPTY)
					continue;
				
				// Check no more rules
				ruleBroken = true;
				break;
			}

			// A '-' means that the rule is satisfied if no tile exists there.
			if (rule == '-')
			{
				// Verify the rule
				if (ruleSpace.type == GridSpaceType::EMPTY)
					continue;

				// Check no more rules
				ruleBroken = true;
				break;
			}

			// A letter means that the rule is satisfied if a tile exists there,
			// with a tag matching the letter.
			{
				// Verify the rule
				if (ruleSpace.type == GridSpaceType::EMPTY)
					continue;

				// Check no more rules
				ruleBroken = true;
				break;
			}
		}



		// Logger::log(std::string("Rule broken: ") + std::to_string(ruleBroken));

		// If we found a non-rule breaking tile, set it as the tile for the spot
		if (!ruleBroken)
		{
			tileIndices[index] = tileIndex;
			tileFound = true;
			break;
		}

		tileIndex++;
	}

	if (!tileFound)
	{
		// Make the tile empty
		tileIndices[index] = -1;
	}
}

void TileGrid::generateVAO()
{
	if (tileIndices == nullptr)
	{
		return;
	}

	unsigned int VBO{ 0 };

	glGenVertexArrays(1, &tileMapVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(tileMapVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(int) * tileGridSize.x * tileGridSize.y * layerCount, tileIndices, GL_STATIC_DRAW);

	glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TileGrid::generateTileIndices()
{
	if (tileIndices != nullptr)
		free(tileIndices);

	// Creating space for the tile indices
	tileIndices = (int*)malloc(tileGridSize.x * tileGridSize.y * layerCount * sizeof(int));

	TileSet* tileSet = TileGridEngine::getTileSet(tileSetName);

	if (tileSet == nullptr)
		return;

	for (unsigned int l{ 0 }; l < layerCount; l++)
	{
		for (unsigned int y{ 0 }; y < tileGridSize.y; y++)
		{
			for (unsigned int x{ 0 }; x < tileGridSize.x; x++)
			{
				GridSpace gridSpace = getGridSpaceAtIndex(x, y, l);

				// Grid space index
				unsigned int index =
					tileGridSize.x * tileGridSize.y * l +
					y * tileGridSize.x +
					x;

				switch (gridSpace.type)
				{
					case GridSpaceType::EMPTY:
						// Indicate an empty space
						tileIndices[index] = -1;
						break;

					case GridSpaceType::FILL_IN:
						autoFillGridSpace(tileSet, x, y, l, '-');
						break;

					case GridSpaceType::TAG:
						autoFillGridSpace(tileSet, x, y, l, gridSpace.tag);
						break;

					case GridSpaceType::INDEX:
						// Use the index directly
						tileIndices[index] = gridSpace.index;
						break;
				}
				std::cout << (tileIndices[index] == -1 ? "-" : std::to_string(tileIndices[index])) << " ";
			}

			std::cout << "\n";
		}
		std::cout << "\n";
	}

	// Putting the new indices into the VAO
	generateVAO();
}
