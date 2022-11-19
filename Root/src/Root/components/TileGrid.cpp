#include "TileGrid.h"

#include <Root/engine/TextureEngine.h>
#include <Root/engine/TileGridEngine.h>

TileGrid::~TileGrid()
{
}

TileGrid* TileGrid::create(Transform* transform,
	unsigned int texture,
	const std::string& dataPath)
{
	TileGrid* tileGrid = new TileGrid(texture, dataPath);
	std::shared_ptr<TileGrid> pointer{ tileGrid };
	transform->addComponent(pointer);
	return tileGrid;
}

void TileGrid::render(float renderDepth)
{
	/*
	// Getting the shader
	Shader* spriteRenderShader{ Root::getTileGridRenderShader() };

	// Setting shader variables
	spriteRenderShader->use();
	spriteRenderShader->setMat4("model", transform->getModelMatrix());
	spriteRenderShader->setMat4("view", Root::getActiveCamera()->getTransform()->getInverseTransformMatrix());

	spriteRenderShader->setMat4("projection", Root::getActiveCamera()->getProjectionMatrix());
	spriteRenderShader->setInt("sprite", 0);
	spriteRenderShader->setFloat("renderDepth", renderDepth / 10000.0f);
	spriteRenderShader->setInt("columnCount", gridSize.x);
	spriteRenderShader->setInt("rowCount", gridSize.y);

	// Binding the sprite
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glBindVertexArray(tileMapVAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);*/
}

unsigned int TileGrid::addTile(glm::ivec2 textureIndex)
{
	return 0;
}

void TileGrid::setTileTextureIndex(unsigned int tileIndex, glm::ivec2 textureIndex)
{
}

TileGrid::TileGrid(unsigned int texture,
	const std::string& dataPath)
	: textureID(texture)
{
	readData(dataPath);
}

void TileGrid::readData(const std::string& dataPath)
{
	bool enableDebugLogging{ true };

	// Reading in the file
	std::ifstream file(dataPath);
	if (!file.is_open())
	{
		Logger::logError("Cannot read tile grid from '" + dataPath + "'; File cannot be opened.");
		return;
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
		tileGridSize.x = TileGridEngine::readNumber(&file, c);
		file.get();
		c = file.get();
		tileGridSize.y = TileGridEngine::readNumber(&file, c);
		if (enableDebugLogging)
			Logger::log(std::string("Found tile grid size: ") + std::to_string(tileGridSize.x) + " x " + std::to_string(tileGridSize.y));
		break;
	}

	if (tileGridSize.x <= 0 || tileGridSize.y <= 0)
	{
		Logger::logError("Tile grid size must not be less than 1 in each dimension.");
		return;
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
		return;
	}

	gridSpaces = (GridSpace*)malloc(tileGridSize.x * tileGridSize.y * layerCount * sizeof(GridSpace));

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
				if (c == ' ' || c == '~')
				{
					// Empty
					gridSpace.type = GridSpaceType::EMPTY;
					if (enableDebugLogging)
						Logger::log("Read empty grid space");
				}
				else
				{
					// Tagged
					gridSpace.type = GridSpaceType::TAG;
					gridSpace.tag = c;
					if (enableDebugLogging)
						Logger::log(std::string("Read grid space with tag: ") + gridSpace.tag);
				}
			}

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
			case GridSpaceType::TAG:
				std::cout << gridSpaces[i].tag << ' ' << std::endl;
				break;
			case GridSpaceType::INDEX:
				std::cout << gridSpaces[i].index << ' ' << std::endl;
				break;
		}
	}
}
