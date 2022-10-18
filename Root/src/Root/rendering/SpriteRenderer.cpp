#include "SpriteRenderer.h"

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
	const char* spritePath, 
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

void SpriteRenderer::setSprite(const char* spritePath, bool pixelPerfect)
{
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	TextureData textureData(spritePath, &width, &height, &nrComponents, 0);

	if (textureData.hasData())
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData.getData());
		glGenerateMipmap(GL_TEXTURE_2D);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, pixelPerfect ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, pixelPerfect ? GL_NEAREST : GL_LINEAR);
	}
	else
	{
		Logger::logError("Failed to load sprite: " + std::string(spritePath) + ".\nReason: " + textureData.getFailureReason());
	}
}

void SpriteRenderer::setSpriteSheetColumnIndex(unsigned int column)
{
	columnIndex = column;

	Logger::log(std::to_string(column) + " - " + std::to_string(Time::getTime()));
}

void SpriteRenderer::setSpriteSheetRowIndex(unsigned int row)
{
	rowIndex = row;
}
