#include "SpriteRenderer.h"

#include <Root/engine/TextureEngine.h>

SpriteRenderer::SpriteRenderer(unsigned int columnCount, unsigned int rowCount)
	: columnCount(columnCount)
	, rowCount(rowCount)
{
}

SpriteRenderer::~SpriteRenderer()
{
	Logger::destructorMessage("Sprite renderer");
}

SpriteRendererPointer SpriteRenderer::create(
	TransformPointer transform,
	const std::string& spritePath,
	bool pixelPerfect,
	unsigned int columnCount,
	unsigned int rowCount)
{
	SpriteRenderer* spriteRenderer = new SpriteRenderer(columnCount, rowCount);
	std::shared_ptr<SpriteRenderer> pointer{ spriteRenderer };
	pointer->setSprite(spritePath, pixelPerfect);
	transform->addComponent(pointer);
	return spriteRenderer;
}

std::string SpriteRenderer::toString()
{
	std::stringstream ss;

	// Writing this object to the stream
	ss << "[SpriteRenderer]"
		<< std::endl;
	return ss.str();
}

void SpriteRenderer::render(float renderDepth)
{
	// Getting the shader
	Shader* spriteRenderShader{ Root::getSpriteRenderShader() };

	// Setting shader variables
	spriteRenderShader->use();
	spriteRenderShader->setMat4("model", transform->getModelMatrix());
	spriteRenderShader->setMat4("view", Root::getActiveCamera()->getTransform()->getInverseTransformMatrix());

	spriteRenderShader->setMat4("projection", Root::getActiveCamera()->getProjectionMatrix());
	spriteRenderShader->setInt("sprite", 0);
	spriteRenderShader->setFloat("renderDepth", renderDepth / 10000.0f);
	spriteRenderShader->setInt("columnCount", columnCount);
	spriteRenderShader->setInt("rowCount", rowCount);
	spriteRenderShader->setInt("columnIndex", columnIndex);
	spriteRenderShader->setInt("rowIndex", rowIndex);

	// Binding the sprite
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBindVertexArray(Renderer::getSquareVAO());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SpriteRenderer::setSprite(const std::string& spritePath, bool pixelPerfect)
{
	textureID = TextureEngine::loadTexture(spritePath, pixelPerfect);
}

void SpriteRenderer::setSpriteSheetColumnIndex(unsigned int column)
{
	if (column >= columnCount)
	{
		Logger::logError("Sprite sheet column index out of range, cannot be larger than "
			+ std::to_string(columnCount-1) + ", was " + std::to_string(column));

		return;
	}

	columnIndex = column;
}

void SpriteRenderer::setSpriteSheetRowIndex(unsigned int row)
{
	if (row >= rowCount)
	{
		Logger::logError("Sprite sheet row index out of range, cannot be larger than "
			+ std::to_string(rowCount - 1) + ", was " + std::to_string(row));

		return;
	}

	rowIndex = row;
}

void SpriteRenderer::setSpriteSheetIndex(glm::ivec2 index)
{
	if (index.y < 0)
	{
		Logger::logError("Sprite sheet row index out of range, cannot be less than 0, was "
			+ std::to_string(index.y));

		return;
	}
	if (index.x < 0)
	{
		Logger::logError("Sprite sheet column index out of range, cannot be less than 0, was "
			+ std::to_string(index.x));

		return;
	}
	setSpriteSheetColumnIndex(index.x);
	setSpriteSheetRowIndex(index.y);
}
