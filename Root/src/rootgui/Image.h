#pragma once

#include "Rectangle.h"

#include <stbi_utility/TextureData.h>

namespace RootGUIComponent
{
	class Image : public RootGUIComponent::Rectangle
	{
	public:
		Image(const char* imagePath,
			unsigned int windowWidth, unsigned int windowHeight,
			glm::vec2 position = glm::vec2(0.0f),
			glm::vec2 size = glm::vec2(1.0f),
			glm::vec2 scale = glm::vec2(1.0f));

		Image(const char* imagePath,
			unsigned int windowWidth, unsigned int windowHeight,
			float imageHeight = 100.0f,
			glm::vec2 position = glm::vec2(0.0f),
			glm::vec2 scale = glm::vec2(1.0f));
		~Image();

		// Render this GUI image
		void render(unsigned int guiShader, unsigned int textShader, unsigned int width, unsigned int height);

	private:

		// Currently loaded image
		unsigned int textureID;

		// Load an image to this component, then get the aspect ratio
		float loadImage(const char* imagePath);
	};
};
