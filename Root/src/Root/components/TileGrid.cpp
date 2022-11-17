#include "TileGrid.h"

#include <Root/engine/TextureEngine.h>

TileGrid::~TileGrid()
{
}

TileGrid* TileGrid::create(Transform* transform,
	unsigned int texture,
	glm::ivec2 gridSize,
	const std::string& dataPath)
{
	TileGrid* tileGrid = new TileGrid(texture, gridSize, dataPath);
	std::shared_ptr<TileGrid> pointer{ tileGrid };
	transform->addComponent(pointer);
	return tileGrid;
}

void TileGrid::render(float renderDepth)
{
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

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

unsigned int TileGrid::addTile(glm::ivec2 textureIndex)
{
	return 0;
}

void TileGrid::setTileTextureIndex(unsigned int tileIndex, glm::ivec2 textureIndex)
{
}

void TileGrid::setTileRule(unsigned int tileIndex, RulePosition rulePosition, TileRule rule)
{
}

TileGrid::TileGrid(unsigned int texture,
	glm::ivec2 gridSize,
	const std::string& dataPath)
	: textureID(texture)
{
}

void TileGrid::readData(const std::string& dataPath)
{
}
