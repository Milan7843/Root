#pragma once

#include "simpletext/SimpleText.h"
#include "Item.h"
#include "Rectangle.h"
#include "Image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace RootGUI
{
	void initialise(unsigned int windowWidth, unsigned int windowHeight);
	void terminate();

	// Render the GUI with the given width and height
	void render(unsigned int width, unsigned int height);

	/**
	 * Get the index of a Vertex Array Object which defines a quad.
	 * 
	 * \returns the index of a Vertex Array Object which defines a quad.
	 */
	unsigned int getQuadVAO();

	// Add an item to the render queue
	void addItemToRenderQueue(RootGUIComponent::Item* item);
};

