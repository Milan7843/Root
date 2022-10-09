#pragma once

#include "simpletext/SimpleText.h"
#include "Item.h"
#include "Rectangle.h"
#include "Image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

class SimpleGUI
{
public:
	SimpleGUI(unsigned int windowWidth, unsigned int windowHeight);
	~SimpleGUI();

	// Render the GUI with the given width and height
	void render(unsigned int width, unsigned int height);

	// Add an item to the render queue
	void addItemToRenderQueue(SimpleGUIComponent::Item* item);

	// Data buffers for a simple quad
	static unsigned int quadVAO;
	static unsigned int quadVBO;
	static unsigned int quadEBO;

private:
	std::vector<SimpleGUIComponent::Item*> renderQueue;

	void createShaderPrograms();
	unsigned int guiShader;
	unsigned int textShader;
};

