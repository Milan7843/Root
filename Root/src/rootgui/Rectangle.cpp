#include "Rectangle.h"

#include "RootGUI.h"

#include "internal/RootGUIInternal.h"

RootGUIComponent::Rectangle::Rectangle(
	glm::vec2 position,
	glm::vec2 size,
	glm::vec2 scale)
	: RootGUIComponent::Item(position, size, scale)
	, color(glm::vec3(1.0f, 0.0f, 1.0f))
{
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
	glm::vec2 screenSize{ getSize() };
	glUniform2f(glGetUniformLocation(guiShader, "position"), screenPosition.x, screenPosition.y);
	glUniform2f(glGetUniformLocation(guiShader, "size"), screenSize.x, screenSize.y);
	glUniform1i(glGetUniformLocation(guiShader, "useTexture"), 0); // Don't use the texture

	glBindVertexArray(RootGUI::getQuadVAO());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
