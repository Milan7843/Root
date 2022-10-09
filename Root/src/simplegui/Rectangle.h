#pragma once

#include "Item.h"

class SimpleGUI;

namespace SimpleGUIComponent
{
	class Rectangle : public SimpleGUIComponent::Item
	{
	public:
		Rectangle(unsigned int windowWidth, unsigned int windowHeight,
			glm::vec2 position = glm::vec2(0.95f, 0.05f),
			glm::vec2 size = glm::vec2(100.0f),
			glm::vec2 scale = glm::vec2(1.0f));
		~Rectangle();

		// Render this GUI rectangle
		void render(unsigned int guiShader, unsigned int textShader, unsigned int width, unsigned int height);

	protected:
		glm::vec3 color;
	};
};

