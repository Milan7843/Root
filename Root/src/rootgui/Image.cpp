#include "Image.h"

#include "RootGUI.h"
#include "internal/RootGUIInternal.h"

RootGUIComponent::Image::Image(const char* imagePath,
    glm::vec2 position, glm::vec2 size, glm::vec2 scale)
    : RootGUIComponent::Rectangle(position, size, scale)
{
	float aspectRatio{ loadImage(imagePath) };
}

RootGUIComponent::Image::Image(const char* imagePath,
	float imageHeight, glm::vec2 position, glm::vec2 scale)
	: RootGUIComponent::Rectangle(position, glm::vec2(imageHeight), scale)
{
	float aspectRatio{ loadImage(imagePath) };

	this->size.x *= aspectRatio;

	std::cout << "r: " << aspectRatio << ", " << size.x << ", " << size.y << std::endl;
}

ImagePointer RootGUIComponent::Image::create(const char* imagePath,
	glm::vec2 position,
	glm::vec2 size,
	glm::vec2 scale)
{
	// Creating a new Image on the heap
	RootGUIComponent::Image* image = 
		new RootGUIComponent::Image(imagePath,
			position,
			size,
			scale);

	// Making a shared pointer from it
	std::shared_ptr<RootGUIComponent::Image> pointer{ image };
	// Adding it to the GUI
	RootGUIInternal::addItemToRenderQueue(pointer);
	return image;
}

ImagePointer RootGUIComponent::Image::create(const char* imagePath,
	float imageHeight,
	glm::vec2 position,
	glm::vec2 scale)
{
	// Creating a new Image on the heap
	RootGUIComponent::Image* image = 
		new RootGUIComponent::Image(imagePath,
			imageHeight,
			position,
			scale);

	// Making a shared pointer from it
	std::shared_ptr<RootGUIComponent::Image> pointer{ image };
	// Adding it to the GUI
	RootGUIInternal::addItemToRenderQueue(pointer);
	return image;
}

RootGUIComponent::Image::~Image()
{
}

void RootGUIComponent::Image::render(unsigned int guiShader, unsigned int textShader)
{
	Item::render(guiShader, textShader);

	glUseProgram(guiShader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Setting the uniforms
	glm::vec2 screenPosition{ getPosition() };
	glUniform2f(glGetUniformLocation(guiShader, "position"), screenPosition.x, screenPosition.y);
	glUniform1i(glGetUniformLocation(guiShader, "useTexture"), 1); // Use the texture
	glUniform1i(glGetUniformLocation(guiShader, "textureSampler"), 0);

	// Draw call
	glBindVertexArray(RootGUI::getQuadVAO());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

float RootGUIComponent::Image::loadImage(const char* imagePath)
{
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	TextureData textureData(imagePath, &width, &height, &nrComponents, 0);

	if (textureData.hasData())
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData.getData());
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		std::cout << "Failed to load image: " << imagePath << ".\nReason: " << textureData.getFailureReason() << std::endl;
	}

	return (float)width / (float)height;
}
