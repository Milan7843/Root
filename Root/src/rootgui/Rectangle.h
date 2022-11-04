#pragma once

#include "Item.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define RectanglePointer RootGUIComponent::Rectangle*

typedef void(*VoidFunction)();

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

		/**
		 * Set the scale reference of this rectangle,
		 * this determines what the rectangle sizes with:
		 * - Width: scale with width
		 * - Height: scale with height
		 */
		void setScaleReference(ScaleReference scaleReference);

		void updateInteractionFlags(glm::vec2 mousePosition, bool mouseDown) override;

		/**
		 * Get whether this rectangle is currently pressed.
		 */
		bool isPressed();

		/**
		 * Get whether this rectangle is currently hovered by the mouse.
		 */
		bool isHovered();

		/**
		 * Set the callback to be called when the rectangle is hovered.
		 * Called when the rectangle was not hovered on the previous frame but is now.
		 * 
		 * \param callback: the callback to be called.
		 */
		void setOnBeginHoverCallback(VoidFunction callback);

		/**
		 * Set the callback to be called when the rectangle is unhovered.
		 * Called when the rectangle was hovered on the previous frame but isn't now.
		 * 
		 * \param callback: the callback to be called.
		 */
		void setOnEndHoverCallback(VoidFunction callback);

		/**
		 * Set the callback to be called when the rectangle is pressed.
		 * Called when the rectangle was not pressed on the previous frame but is now.
		 * 
		 * \param callback: the callback to be called.
		 */
		void setOnBeginPressCallback(VoidFunction callback);

		/**
		 * Set the callback to be called when the rectangle is unpressed.
		 * Called when the rectangle was pressed on the previous frame but isn't now.
		 * 
		 * \param callback: the callback to be called.
		 */
		void setOnEndPressCallback(VoidFunction callback);

	protected:

		Rectangle(
			glm::vec2 position,
			glm::vec2 size,
			glm::vec2 scale);

		void updateTransformMatrices();
		// The transform matrix is for transforming a point from screen space to local space
		glm::mat4& getTransformMatrix();
		// The inverse transform matrix is for transforming a point from local space to screen space
		glm::mat4& getInverseTransformMatrix();
		glm::mat4 transform{ glm::identity<glm::mat4>() };
		glm::mat4 inverseTransform{ glm::identity<glm::mat4>() };
		bool transformUpdated{ true };

		glm::vec3 color;

		// Scale of this item: applied after everything else
		glm::vec2 scale;

		// Size of this item
		glm::vec2 size;

		bool hovered;
		bool pressed;

		// Callbacks
		// For hover
		VoidFunction onBeginHoverCallback{ nullptr };
		VoidFunction onEndHoverCallback{ nullptr };

		// For press
		VoidFunction onBeginPressCallback{ nullptr };
		VoidFunction onEndPressCallback{ nullptr };

		virtual void callOnBeginHoverCallback();
		virtual void callOnEndHoverCallback();
		virtual void callOnBeginPressCallback();
		virtual void callOnEndPressCallback();

		ScaleReference scaleReference{ ScaleReference::Height };
	};
};

