#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

enum class VerticalAnchorPoint
{
	Bottom,
	Middle,
	Top
};

enum class HorizontalAnchorPoint
{
	Left,
	Middle,
	Right
};

namespace RootGUIComponent
{
	class Item
	{
	public:
		~Item();

		// Render this GUI item
		virtual void render(unsigned int guiShader, unsigned int textShader) {
		}

		glm::vec2 getPosition();

		// Set the vertical screen anchor point of this item
		void setVerticalScreenAnchorPoint(VerticalAnchorPoint newVerticalScreenAnchorPoint);
		void setHorizontalScreenAnchorPoint(HorizontalAnchorPoint newHorizontalScreenAnchorPoint);

	protected:

		Item(glm::vec2 position = glm::vec2(0.1f));

		float getVerticalScreenAnchor();
		float getHorizontalScreenAnchor();

		bool enabled{ true };

		// Position on the screen
		glm::vec2 position;

		// Where on the screen this rectangle should be placed relative to
		VerticalAnchorPoint verticalAnchorPoint{ VerticalAnchorPoint::Middle };
		HorizontalAnchorPoint horizontalAnchorPoint{ HorizontalAnchorPoint::Middle };

	private:

		unsigned int previousWindowHeight{ 0 };
		unsigned int previousWindowWidth{ 0 };

		float widthToHeightRatio;
	};
};
