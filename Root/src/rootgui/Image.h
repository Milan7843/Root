#pragma once

#include "Rectangle.h"

#include <stbi_utility/TextureData.h>

#define ImagePointer RootGUIComponent::Image*

namespace RootGUIComponent
{
	class Image : public RootGUIComponent::Rectangle
	{
	public:

		/**
		 * Create a new Image.
		 * 
		 * \param imagePath: the path to the image.
		 * \param position: the position of the image (default = [0, 0]).
		 * \param size: the size of the image (default = [1, 1]).
		 * \param rotation: the rotation of the image (default = 0).
		 * \return a pointer to the new image.
		 */
		static ImagePointer create(const char* imagePath,
			glm::vec2 position = glm::vec2(0.0f),
			glm::vec2 size = glm::vec2(1.0f),
			float rotation = 0.0f);

		/**
		 * Create a new Image.
		 *
		 * \param imagePath: the path to the image.
		 * \param imageHeight: the height of the image (default = 100px).
		 * \param position: the position of the image (default = [0, 0]).
		 * \param rotation: the rotation of the image (default = 0).
		 * \return a pointer to the new image.
		 */
		ImagePointer create(const char* imagePath,
			float imageHeight = 100.0f,
			glm::vec2 position = glm::vec2(0.0f),
			float rotation = 0.0f);


		~Image();

		// Render this GUI image
		void render(unsigned int guiShader, unsigned int textShader);

	private:

		// Currently loaded image
		unsigned int textureID;

		// Load an image to this component, then get the aspect ratio
		float loadImage(const char* imagePath);

		Image(const char* imagePath,
			glm::vec2 position,
			glm::vec2 size,
			float rotation);

		Image(const char* imagePath,
			float imageHeight,
			glm::vec2 position,
			float rotation);
	};
};
