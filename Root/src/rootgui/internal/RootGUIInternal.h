#pragma once

#include <rootgui/internal/TextEngine.h>
#include "rootgui/Item.h"
#include "rootgui/Rectangle.h"
#include "rootgui/Image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace RootGUIInternal
{
	void initialise(unsigned int windowWidth, unsigned int windowHeight);

	/**
	 * Update the GUI. Necessary for GUI interaction.
	 */
	void update(glm::vec2 mousePosition, bool mouseDown);

	void terminate();

	// Render the GUI with the given width and height
	void render();


	unsigned int getTextShader();

	unsigned int getTextDebugShader();

	/**
	 * Get the index of a Vertex Array Object which defines a quad.
	 *
	 * \returns the index of a Vertex Array Object which defines a quad.
	 */
	unsigned int getQuadVAO();

	glm::mat4& getProjectionMatrix();

	/**
	 * Add a new item to the render queue.
	 *
	 * \param item: the item to add to the render queue.
	 */
	void addItemToRenderQueue(std::shared_ptr<RootGUIComponent::Item> item);

	/**
	 * Set the size of the window.
	 * 
	 * \param windowWidth: the width of the window in pixels.
	 * \param windowHeight: the height of the window in pixels.
	 */
	void setWindowSize(unsigned int windowWidth, unsigned int windowHeight);

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
	 * Enable the debug view mode.
	 */
	void enableDebugMode();

	/**
	 * Disable the debug view mode.
	 */
	void disableDebugMode();

	/**
	 * Get whether the GUI is in debug mode.
	 */
	bool isInDebugMode();
};



