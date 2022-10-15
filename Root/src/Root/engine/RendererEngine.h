#pragma once

#include <Root/rendering/ScreenSpaceEffect.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>


namespace RendererEngine
{
	/**
	 * Initialise the renderer.
	 */
	void initialise(unsigned int width, unsigned int height);

	/**
	 * Terminate the renderer.
	 */
	void terminate();

	/**
	 * Start a new frame.
	 */
	void newFrame();

	/**
	 * Display the frame.
	 */
	void displayFrame();

	/**
	 * Add a new screen space effect.
	 */
	void addScreenSpaceEffect(ScreenSpaceEffectPointer screenSpaceEffect);

	/**
	 * Run every screen space effect.
	 */
	void runScreenSpaceEffects();

	/**
	 * Set the resolution used for rendering the textures.
	 */
	void setTextureSize(unsigned int width, unsigned int height);

	/**
	 * Set the quality level of the MSAA.
	 *
	 * \param qualityLevel: the new MSAA quality level.
	 */
	void setMSAAQualityLevel(unsigned int qualityLevel);

	/**
	 * Get the index of a vertex array object which holds information to draw a square.
	 */
	unsigned int getSquareVAO();

	/**
	 * Get the index of a vertex array object which holds information to draw a screen overlay rectangle.
	 */
	unsigned int getScreenRectVAO();
};
