#include "Rectangle.h"

#include "RootGUI.h"

#include "internal/RootGUIInternal.h"

RootGUIComponent::Rectangle::Rectangle(
	glm::vec2 position,
	glm::vec2 size,
	glm::vec2 scale)
	: RootGUIComponent::Item(position)
	, color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0))
	, size(size)
	, scale(scale)
{
	scaleDifferenceOnInteract.addValue(InteractionStatus::None, glm::vec2(1.0f));
	colorDifferenceOnInteract.addValue(InteractionStatus::None, glm::vec4(1.0f));
}

void RootGUIComponent::Rectangle::updateTransformMatrices()
{
	// Calculating important information
	float aspectRatio{ (float)RootGUIInternal::getWindowWidth() / (float)RootGUIInternal::getWindowHeight() };
	glm::vec3 screenAnchorPoint{ glm::vec3(getHorizontalScreenAnchor(), getVerticalScreenAnchor(), 0.0f) };

	// Resetting the transform
	inverseTransform = glm::identity<glm::mat4>();

	inverseTransform = glm::translate(inverseTransform, screenAnchorPoint);

	if (scaleReference == ScaleReference::Height)
	{
		inverseTransform = glm::scale(inverseTransform, glm::vec3(1.0f / aspectRatio, 1.0f, 1.0f));
	}
	else
	{
		inverseTransform = glm::scale(inverseTransform, glm::vec3(1.0f, aspectRatio, 1.0f));
	}

	inverseTransform = glm::translate(inverseTransform, glm::vec3(position.x, position.y, 0.0f));

	//inverseTransform = glm::scale(inverseTransform, glm::vec3(size.x, size.y, 1.0f));


	// Resetting the inverse transform
	transform = glm::identity<glm::mat4>();

	// Scale must not be zero for this step
	//if (size.x != 0.0f && size.y != 0.0f)
	//	transform = glm::scale(transform, glm::vec3(1.0f / size.x, 1.0f / size.y, 1.0f));

	transform = glm::translate(transform, glm::vec3(-position.x, -position.y, 0.0f));

	if (scaleReference == ScaleReference::Height)
	{
		transform = glm::scale(transform, glm::vec3(aspectRatio, 1.0f, 1.0f));
	}
	else
	{
		transform = glm::scale(transform, glm::vec3(1.0f, 1.0f / aspectRatio, 1.0f));
	}

	transform = glm::translate(transform, -screenAnchorPoint);
}

glm::mat4& RootGUIComponent::Rectangle::getTransformMatrix()
{
	// Recalculating the transform matrices if the transform data was updated
	if (transformUpdated)
	{
		updateTransformMatrices();
	}

	return transform;
}

void RootGUIComponent::Rectangle::setColor(glm::vec4 color)
{
	this->color = color;
}

void RootGUIComponent::Rectangle::setColor(glm::vec3 color)
{
	this->color = glm::vec4(color, 1.0f);
}

void RootGUIComponent::Rectangle::setColorOnHover(glm::vec4 color)
{
	colorDifferenceOnInteract.addValue(InteractionStatus::Hovered, color);
}

void RootGUIComponent::Rectangle::setColorOnHover(glm::vec3 color)
{
	colorDifferenceOnInteract.addValue(InteractionStatus::Hovered, glm::vec4(color, 1.0f));
}

void RootGUIComponent::Rectangle::setColorOnPress(glm::vec4 color)
{
	colorDifferenceOnInteract.addValue(InteractionStatus::Pressed, color);
}

void RootGUIComponent::Rectangle::setColorOnPress(glm::vec3 color)
{
	colorDifferenceOnInteract.addValue(InteractionStatus::Pressed, glm::vec4(color, 1.0f));
}

glm::mat4& RootGUIComponent::Rectangle::getInverseTransformMatrix()
{
	// Recalculating the transform matrices if the transform data was updated
	if (transformUpdated)
	{
		updateTransformMatrices();
	}

	return inverseTransform;
}

RectanglePointer RootGUIComponent::Rectangle::create(
	glm::vec2 position,
	glm::vec2 size,
	glm::vec2 scale)
{
	// Creating a new Rectangle on the heap
	RootGUIComponent::Rectangle* rectangle = 
		new RootGUIComponent::Rectangle(position, size, scale);

	// Creating a shared pointer from it
	std::shared_ptr<RootGUIComponent::Rectangle> pointer{ rectangle };
	// And adding it to the GUI
	RootGUIInternal::addItemToRenderQueue(pointer);
	return rectangle;
}

RootGUIComponent::Rectangle::~Rectangle()
{
}

void RootGUIComponent::Rectangle::render(unsigned int guiShader,
	unsigned int textShader)
{
	Item::render(guiShader, textShader);

	glm::vec2 scaleDifferenceByInteract{ scaleDifferenceOnInteract.sample() };

	glUseProgram(guiShader);

	glUniform2f(glGetUniformLocation(guiShader, "additionalScale"),
		scaleDifferenceByInteract.x, scaleDifferenceByInteract.y);

	// Setting the color uniform
	glm::vec4 colorDifferenceByInteract{ colorDifferenceOnInteract.sample() };
	glm::vec4 colorUsing{ color * colorDifferenceByInteract };
	glUniform4f(glGetUniformLocation(guiShader, "baseColor"),
		colorUsing.r, colorUsing.g, colorUsing.b, colorUsing.a);

	glUniform2f(glGetUniformLocation(guiShader, "size"), size.x, size.y);
	glm::vec2 screenPosition{ getPosition() };

	glUniform1i(glGetUniformLocation(guiShader, "useTexture"), 0); // Don't use the texture

	// Setting the transform matrix
	glUniformMatrix4fv(glGetUniformLocation(guiShader, "transform"),
		1, GL_FALSE,
		glm::value_ptr(getInverseTransformMatrix()));

	glBindVertexArray(RootGUI::getQuadVAO());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void RootGUIComponent::Rectangle::setScaleReference(ScaleReference scaleReference)
{
	this->scaleReference = scaleReference;
}

void RootGUIComponent::Rectangle::updateInteractionFlags(glm::vec2 mousePosition, bool mouseDown)
{
	glm::vec2 localPosition(getTransformMatrix() * glm::vec4(mousePosition.x, mousePosition.y, 0.0f, 1.0f));

	if (localPosition.x >= -size.x / 2.0f && localPosition.x <= size.x / 2.0f &&
		localPosition.y >= -size.y / 2.0f && localPosition.y <= size.y / 2.0f)
	{
		// Local mouse position is within the bounds
		if (mouseDown)
		{
			// If this is the first time the rectangle was pressed
			if (!pressed)
			{
				// Calling the on begin press callback
				callOnBeginPressCallback();
			}

			pressed = true;
			hovered = true;
			setInteractionStatus(InteractionStatus::Pressed);
		}
		else
		{
			if (pressed)
			{
				// Rectangle was pressed last update, but not now
				callOnEndPressCallback();
			}

			// If this is the first time the rectangle was hovered
			if (!hovered)
			{
				// Calling the on begin press callback
				callOnBeginHoverCallback();
			}

			hovered = true;
			pressed = false;
			setInteractionStatus(InteractionStatus::Hovered);
		}
	}
	else // Rectangle is not being hovered
	{
		if (pressed)
		{
			// Rectangle was pressed last update, but not now
			callOnEndPressCallback();
		}

		if (hovered)
		{
			// Rectangle was hovered last update, but not now
			callOnEndHoverCallback();
		}

		// Rectangle is not pressed nor hovered
		pressed = false;
		hovered = false;
		setInteractionStatus(InteractionStatus::None);
	}
}

void RootGUIComponent::Rectangle::setOnBeginHoverCallback(VoidFunction callback)
{
	onBeginHoverCallback = callback;
}

void RootGUIComponent::Rectangle::setOnEndHoverCallback(VoidFunction callback)
{
	onEndHoverCallback = callback;
}

void RootGUIComponent::Rectangle::setOnBeginPressCallback(VoidFunction callback)
{
	onBeginPressCallback = callback;
}

void RootGUIComponent::Rectangle::setOnEndPressCallback(VoidFunction callback)
{
	onEndPressCallback = callback;
}

void RootGUIComponent::Rectangle::callOnBeginHoverCallback()
{
	if (onBeginHoverCallback != nullptr)
	{
		onBeginHoverCallback();
	}
}

void RootGUIComponent::Rectangle::callOnEndHoverCallback()
{
	if (onEndHoverCallback != nullptr)
	{
		onEndHoverCallback();
	}
}

void RootGUIComponent::Rectangle::callOnBeginPressCallback()
{
	if (onBeginPressCallback != nullptr)
	{
		onBeginPressCallback();
	}
}

void RootGUIComponent::Rectangle::callOnEndPressCallback()
{
	if (onEndPressCallback != nullptr)
	{
		onEndPressCallback();
	}
}

void RootGUIComponent::Rectangle::setInteractionStatus(InteractionStatus status)
{
	scaleDifferenceOnInteract.setActiveValue(status);
	colorDifferenceOnInteract.setActiveValue(status);
}

void RootGUIComponent::Rectangle::setScaleOnHover(glm::vec2 scale)
{
	scaleDifferenceOnInteract.addValue(InteractionStatus::Hovered, scale);
}


void RootGUIComponent::Rectangle::setScaleOnPress(glm::vec2 scale)
{
	scaleDifferenceOnInteract.addValue(InteractionStatus::Pressed, scale);
}

void RootGUIComponent::Rectangle::setTransitionDuration(float transitionDuration)
{
	scaleDifferenceOnInteract.setTransitionDuration(transitionDuration);
	colorDifferenceOnInteract.setTransitionDuration(transitionDuration);
}

bool RootGUIComponent::Rectangle::isPressed()
{
	return pressed;
}

bool RootGUIComponent::Rectangle::isHovered()
{
	return hovered;
}

InteractionStatus RootGUIComponent::Rectangle::getInteractionStatus()
{
	if (pressed)
	{
		return InteractionStatus::Pressed;
	}
	if (hovered)
	{
		return InteractionStatus::Hovered;
	}
	return InteractionStatus::None;
}
