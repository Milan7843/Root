#pragma once

#include "Item.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define RectanglePointer RootGUIComponent::Rectangle*

enum class ScaleReference
{
	Width,
	Height
};

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

		void setInternalAnchor(glm::vec2 anchor);

		void setScaleReference(ScaleReference scaleReference);

	protected:

		Rectangle(
			glm::vec2 position,
			glm::vec2 size,
			glm::vec2 scale);

		glm::mat4& getTransformMatrix();
		glm::mat4 transform{ glm::identity<glm::mat4>() };
		bool transformUpdated{ true };

		glm::vec3 color;

		// Scale of this item: applied after everything else
		glm::vec2 scale;

		// Size of this item
		glm::vec2 size;

		//glm::mat4 modelMatrix{};
		glm::vec2 anchor{ glm::vec2(0.5f) };

		ScaleReference scaleReference{ ScaleReference::Height };
	};
};

