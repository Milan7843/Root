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
	/**
	 * Get the index of a Vertex Array Object which defines a quad.
	 * 
	 * \returns the index of a Vertex Array Object which defines a quad.
	 */
	unsigned int getQuadVAO();

	/**
	 * Add a new item to the render queue.
	 * 
	 * \param item: the item to add to the render queue.
	 */
	void addItemToRenderQueue(RootGUIComponent::Item* item);
};

