#pragma once

#include "Root/Root.h"
#include "Root/Logger.h"
#include "Root/components/Component.h"
#include "Root/shaders/Shader.h"
#include "Root/rendering/Renderer.h"
#include "Root/rendering/Camera.h"

#include <stbi_utility/TextureData.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define SpriteRendererPointer SpriteRenderer*

/**
 * Component for rendering sprites.
 */
class SpriteRenderer : public Component
{
public:
	~SpriteRenderer();

	/**
	 * Create a new sprite renderer.
	 * Will automatically add this component to the given transform.
	 * 
	 * \param transform: the transform to add this component to.
	 * \param offset: the offset of the image relative to the transform center (default = (0, 0)).
	 * \param pixelPerfect: whether the sprite should be pixel perfect (default = false).
	 * \param columnCount: the number of columns in this sprite (sheet).
	 * \param rowCount: the number of rows in this sprite (sheet).
	 */
	static SpriteRendererPointer create(TransformPointer transform,
		const std::string& spritePath,
		glm::vec2 offset = glm::vec2(0.0f),
		bool pixelPerfect = false,
		unsigned int columnCount = 1, unsigned int rowCount = 1);

	/**
	 * Get a string representing this Component.
	 *
	 * \returns this Component in a human-readable format.
	 */
	std::string toString() override;

	void render(float renderDepth) override;

	/**
	 * Set the sprite used by this sprite renderer.
	 * 
	 * \param spritePath: the path to the sprite to set.
	 * \param pixelPerfect: whether the sprite should be pixelated or not.
	 */
	void setSprite(const std::string& spritePath, bool pixelPerfect);

	/**
	 * Set the column that is being used from the sprite sheet.
	 *
	 * \param column: the index of the column to be used.
	 */
	void setSpriteSheetColumnIndex(unsigned int column);

	/**
	 * Set the row that is being used from the sprite sheet.
	 * 
	 * \param row: the index of the row to be used.
	 */
	void setSpriteSheetRowIndex(unsigned int row);

	/**
	 * Set the row and column that are being used from the sprite sheet.
	 *
	 * \param index: the 2D index of the sprite sheet to be used (column, row).
	 */
	void setSpriteSheetIndex(glm::ivec2 index);

	/**
	 * Set the offset of the rendered sprite relative to the transform center.
	 * 
	 * \param offset: the new offset.
	 */
	void setOffset(glm::vec2 offset);

private:

	SpriteRenderer(unsigned int columnCount, unsigned int rowCount,
		glm::vec2 offset);

	// float height;

	unsigned int textureID{ 0 };

	unsigned int columnCount;
	unsigned int rowCount;

	unsigned int columnIndex{ 0 };
	unsigned int rowIndex{ 0 };

	glm::vec2 offset;
};
