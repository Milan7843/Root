#include "Rectangle.h"

#include "RootGUI.h"

RootGUIComponent::Rectangle::Rectangle(unsigned int windowWidth, unsigned int windowHeight, 
	glm::vec2 position, glm::vec2 size, glm::vec2 scale)
	: RootGUIComponent::Item(windowWidth, windowHeight, position, size, scale)
	, color(glm::vec3(1.0f, 0.0f, 1.0f))
{
}

RootGUIComponent::Rectangle::~Rectangle()
{
}

void RootGUIComponent::Rectangle::render(unsigned int guiShader, unsigned int textShader, unsigned int width, unsigned int height)
{
	Item::render(guiShader, textShader, width, height);

	glUseProgram(guiShader);

	// Setting the color uniform
	glUniform3f(glGetUniformLocation(guiShader, "baseColor"), color.x, color.y, color.z);
	glm::vec2 screenPosition{ getPosition(width, height) };
	glm::vec2 screenSize{ getSize(width, height) };
	glUniform2f(glGetUniformLocation(guiShader, "position"), screenPosition.x, screenPosition.y);
	glUniform2f(glGetUniformLocation(guiShader, "size"), screenSize.x, screenSize.y);
	glUniform1i(glGetUniformLocation(guiShader, "useTexture"), 0); // Don't use the texture

	glBindVertexArray(RootGUI::getQuadVAO());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}
