#include "Rectangle.h"

#include "RootGUI.h"

#include "internal/RootGUIInternal.h"

RootGUIComponent::Rectangle::Rectangle(
	glm::vec2 position,
	glm::vec2 size,
	glm::vec2 scale)
	: RootGUIComponent::Item(position)
	, color(glm::vec3(1.0f, 0.0f, 1.0f))
	, size(size)
	, scale(scale)
{
}

glm::mat4& RootGUIComponent::Rectangle::getTransformMatrix()
{
	// Recalculating the transform matrix if the transform data was updated
	if (transformUpdated)
	{
		// Calculating important information
		float aspectRatio{ (float)RootGUIInternal::getWindowWidth() / (float)RootGUIInternal::getWindowHeight() };
		glm::vec3 screenAnchorPoint{ glm::vec3(getHorizontalScreenAnchor(), getVerticalScreenAnchor(), 0.0f) };

		// Resetting the transform
		transform = glm::identity<glm::mat4>();

		transform = glm::translate(transform, screenAnchorPoint);

		if (scaleReference == ScaleReference::Height)
		{
			transform = glm::scale(transform, glm::vec3(1.0f / aspectRatio, 1.0f, 1.0f));
		}
		else
		{
			transform = glm::scale(transform, glm::vec3(1.0f, aspectRatio, 1.0f));
		}

		transform = glm::translate(transform, glm::vec3(position.x, position.y, 0.0f));

		transform = glm::scale(transform, glm::vec3(size.x, size.y, 1.0f));
	}

	return transform;
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

	glUseProgram(guiShader);

	// Setting the color uniform
	glUniform3f(glGetUniformLocation(guiShader, "baseColor"), color.x, color.y, color.z);
	glm::vec2 screenPosition{ getPosition() };

	glUniform1i(glGetUniformLocation(guiShader, "useTexture"), 0); // Don't use the texture

	// Setting the transform matrix
	glUniformMatrix4fv(glGetUniformLocation(guiShader, "transform"),
		1, GL_FALSE,
		glm::value_ptr(getTransformMatrix()));

	glBindVertexArray(RootGUI::getQuadVAO());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void RootGUIComponent::Rectangle::setInternalAnchor(glm::vec2 anchor)
{
	this->anchor = anchor;
}

void RootGUIComponent::Rectangle::setScaleReference(ScaleReference scaleReference)
{
	this->scaleReference = scaleReference;
}
