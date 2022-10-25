#pragma once

#include "Root/engine/RootEngine.h"
#include "Root/rendering/Camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

namespace InputEngine
{
	/**
	 * Initialise the input engine.
	 */
	void initialise();

	/**
	 * Update the input engine.
	 * Sets variables to enable key detection.
	 */
	void update();

	/**
	 * Indicate a new frame.
	 * Resets variables for input detection
	 */
	void newFrame();

	/**
	 * Get whether the key is currently pressed.
	 * Returns true on every frame the key is down.
	 *
	 * \param key: the key to check for. Use 'KEY_' followed by the desired key to specify a key, e.g. KEY_A.
	 * \return whether the key is down for the first frame.
	 */
	bool getKey(int key);

	/**
	 * Get whether the key was pressed.
	 * Returns true on the frame the key is pressed.
	 *
	 * \param key: the key to check for. Use 'KEY_' followed by the desired key to specify a key, e.g. KEY_A.
	 * \return whether the key is pressed.
	 */
	bool getKeyPressed(int key);

	/**
	 * Get whether the key was released.
	 * Returns true on the frame the key is released.
	 *
	 * \param key: the key to check for. Use 'KEY_' followed by the desired key to specify a key, e.g. KEY_A.
	 * \return whether the key is released.
	 */
	bool getKeyReleased(int key);

	/**
	 * Get whether the mouse button is currently pressed.
	 * Returns true on every frame the mouse button is down.
	 *
	 * \param mouseButton: the mouse button to check for. Use 'MOUSE_' followed by the desired key to specify a key, e.g. MOUSE_LEFT.
	 * \return whether the button is down for the first frame.
	 */
	bool getMouseButton(int mouseButton);

	/**
	 * Get whether the mouse button was pressed.
	 * Returns true on the frame the mouse button is pressed.
	 *
	 * \param mouseButton: the mouse button to check for. Use 'MOUSE_' followed by the desired key to specify a key, e.g. MOUSE_LEFT.
	 * \return whether the key is pressed.
	 */
	bool getMouseButtonPressed(int mouseButton);

	/**
	 * Get whether the mouse button was released.
	 * Returns true on the frame the mouse button is released.
	 *
	 * \param mouseButton: the mouse button to check for. Use 'MOUSE_' followed by the desired key to specify a key, e.g. MOUSE_LEFT.
	 * \return whether the mouse button is released.
	 */
	bool getMouseButtonReleased(int mouseButton);

	/**
	 * Get the current location of the mouse.
	 *
	 * \return the location of the mouse.
	 */
	glm::vec2 getMousePosition();

	/**
	 * Get the change in the mouse position since last frame.
	 *
	 * \return the change in the mouse position since last frame.
	 */
	glm::vec2 getMouseDelta();

	/**
	 * Get the current location of the mouse relative to the world.
	 *
	 * \return the world location of the mouse.
	 */
	glm::vec2 getMouseWorldPosition();
};

