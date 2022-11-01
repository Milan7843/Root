#pragma once

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
	void addItemToRenderQueue(std::shared_ptr<RootGUIComponent::Item> item);

	/**
	 * Get the current width of the window in pixels.
	 * 
	 * \returns the current width of the window.
	 */
	unsigned int getWindowWidth();

	/**
	 * Get the current height of the window in pixels.
	 *
	 * \returns the current height of the window.
	 */
	unsigned int getWindowHeight();

	/**
	 * Load the data for a font.
	 * 
	 * \param fontPath: the path to the font.
	 * \param tag: the tag to identify the font by from now.
	 */
	void loadFont(const std::string& fontPath, const std::string& tag);
};

