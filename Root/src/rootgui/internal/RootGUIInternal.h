#pragma once

#include "simpletext/SimpleText.h"
#include "rootgui/Item.h"
#include "rootgui/Rectangle.h"
#include "rootgui/Image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace RootGUIInternal
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

