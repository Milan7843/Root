#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

enum class VerticalAnchorPoint {
	Bottom,
	Middle,
	Top
};

enum class HorizontalAnchorPoint {
	Left,
	Middle,
	Right
};

namespace RootGUIComponent
{
	class Item
	{
	public:
		Item(
			glm::vec2 position = glm::vec2(0.1f),
			glm::vec2 size = glm::vec2(0.1f),
			glm::vec2 scale = glm::vec2(1.0f));
		~Item();

		// Render this GUI item
		virtual void render(unsigned int guiShader, unsigned int textShader) {
		}

		// Set the vertical anchor point of this item
		void setVerticalAnchorPoint(VerticalAnchorPoint newVerticalAnchorPoint);
		void setHorizontalAnchorPoint(HorizontalAnchorPoint newHorizontalAnchorPoint);

		glm::vec2 getPosition();

		glm::vec2 getSize();

	protected:

		// Scale of this item: applied after everything else
		glm::vec2 scale;

		// Size of this item
		glm::vec2 size;

		bool enabled{ true };

	private:

		VerticalAnchorPoint verticalAnchorPoint{ VerticalAnchorPoint::Bottom };
		HorizontalAnchorPoint horizontalAnchorPoint{ HorizontalAnchorPoint::Right };

		unsigned int previousWindowHeight{ 0 };
		unsigned int previousWindowWidth{ 0 };

		float widthToHeightRatio;
		
		// Position on the screen
		glm::vec2 position;
	};
};
