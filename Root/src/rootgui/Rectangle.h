#pragma once

#include <rootgui/Item.h>

#include <Root/base/TransformBase.h>
#include <Root/InterpolatedValue.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define RectanglePointer RootGUIComponent::Rectangle*

typedef void(*VoidFunction)();

enum class ScaleReference
{
	Width,
	Height
};

enum class InteractionStatus
{
	None,
	Hovered,
	Pressed
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
		 * \param size: the size of the rectangle (default = [1, 1]).
		 * \parma rotation: rotation in degrees (default = 0).
		 * \returns a pointer to the new rectangle.
		 */
		static RectanglePointer create(
			glm::vec2 position = glm::vec2(0.95f, 0.05f),
			glm::vec2 size = glm::vec2(1.0f),
			float rotation = 0.0f);

		~Rectangle();

		// Render this GUI rectangle
		void render(unsigned int guiShader, unsigned int textShader);

		/**
		 * Set the color of the rectangle.
		 * 
		 * \param color: the color with alpha for transparency.
		 */
		void setColor(glm::vec4 color);

		/**
		 * Set the color of the rectangle.
		 *
		 * \param color: the color.
		 */
		void setColor(glm::vec3 color);

		/**
		 * Set the color of the rectangle on hover.
		 *
		 * \param color: the color with alpha for transparency.
		 */
		void setColorOnHover(glm::vec4 color);

		/**
		 * Set the color of the rectangle on hover.
		 *
		 * \param color: the color.
		 */
		void setColorOnHover(glm::vec3 color);

		/**
		 * Set the color of the rectangle on press.
		 *
		 * \param color: the color with alpha for transparency.
		 */
		void setColorOnPress(glm::vec4 color);

		/**
		 * Set the color of the rectangle on press.
		 *
		 * \param color: the color.
		 */
		void setColorOnPress(glm::vec3 color);

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
		 * Get the current interaction status.
		 */
		InteractionStatus getInteractionStatus();

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

		/**
		 * Set the scale of the rectangle on hover.
		 */
		void setScaleOnHover(glm::vec2 scale);

		/**
		 * Set the scale of the rectangle on press.
		 */
		void setScaleOnPress(glm::vec2 scale);

		/**
		 * Set the transition duration for scale and color transition.
		 *
		 * \param transitionDuration: the new transition duration.
		 */
		virtual void setTransitionDuration(float transitionDuration);

		/**
		 * Get the parent of this GUI item.
		 *
		 * \returns the parent of this GUI item, or nullptr if it does not have one.
		 */
		Rectangle* getParent();

		/**
		 * Set the parent transform of this transform.
		 * Having a parent will make the child transform's data in local space.
		 * This will also add this transform as a child to the parent.
		 * Note that adding a child to a transform via Transform::addChild()
		 * will also set the child's parent using setParent(),
		 * and therefore does not have to be called manually.
		 *
		 * \param parent: a pointer to the transform that will be the parent.
		 * If this is NULL, the transform will have no parent.
		 */
		void setParent(Rectangle* parent, bool alsoAddChild = true);

		/**
		 * Add a child to this transform.
		 * This will also set the child's parent pointer to this transform.
		 * Note that setting a transform's parent via Transform::setParent()
		 * will also set the parent's child using addChild(),
		 * and therefore does not have to be called manually.
		 *
		 * \param child: a pointer to the child.
		 */
		void addChild(Rectangle* child, bool alsoSetParent = true);

		/**
		 * Remove a child from this transform with the given shared_ptr.
		 * This will also set the child's parent pointer to NULL, indicating no parent.
		 *
		 * \param childToRemove: a TransformPointer to the child that should be removed.
		 * \returns whether a child was removed succesfully.
		 */
		bool removeChild(Rectangle* childToRemove);

	protected:

		Rectangle(
			glm::vec2 position,
			glm::vec2 size,
			float rotation);

		void updateTransformMatrices() override;

		/*
		// The transform matrix is for transforming a point from screen space to local space
		glm::mat4& getTransformMatrix();
		// The inverse transform matrix is for transforming a point from local space to screen space
		glm::mat4& getInverseTransformMatrix();
		glm::mat4 transform{ glm::identity<glm::mat4>() };
		glm::mat4 inverseTransform{ glm::identity<glm::mat4>() };
		bool transformUpdated{ true };
		*/

		glm::vec4 color;

		// Scale of this item: applied after everything else
		//glm::vec2 scale;
		//glm::vec2 scale;

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

		virtual void setInteractionStatus(InteractionStatus status);

		ScaleReference scaleReference{ ScaleReference::Height };

		InterpolatedValue<InteractionStatus, glm::vec2> scaleDifferenceOnInteract{ 0.2f };
		InterpolatedValue<InteractionStatus, glm::vec4> colorDifferenceOnInteract{ 0.2f };

		Rectangle* derivedParent = nullptr;
		std::vector<Rectangle*> derivedChildren;
	};
};

