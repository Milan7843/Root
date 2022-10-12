#pragma once

#include "Item.h"

#define RectanglePointer RootGUIComponent::Rectangle*

namespace RootGUIComponent
{
	class Rectangle : public RootGUIComponent::Item
	{
	public:

		/**
		 * Create a new rectangle.
		 * 
		 * \param position: the position of the rectangle (default = [0, 0]).
		 * \param size: the size of the rectangle (default = [100, 100]).
		 * \param scale: the position of the rectangle (default = [1, 1]).
		 * \returns a pointer to the new rectangle.
		 */
		static RectanglePointer create(
			glm::vec2 position = glm::vec2(0.95f, 0.05f),
			glm::vec2 size = glm::vec2(100.0f),
			glm::vec2 scale = glm::vec2(1.0f));

		~Rectangle();

		// Render this GUI rectangle
		void render(unsigned int guiShader, unsigned int textShader);

	protected:

		glm::vec3 color;

		Rectangle(
			glm::vec2 position,
			glm::vec2 size,
			glm::vec2 scale);
	};
};

