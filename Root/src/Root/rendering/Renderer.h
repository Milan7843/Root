#pragma once

#include <Root/rendering/ScreenSpaceEffect.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>


namespace Renderer
{
	/**
	 * Initialise the renderer.
	 */
	void initialise(unsigned int width, unsigned int height);

	/**
	 * Add a new screen space effect.
	 * 
	 * \returns a pointer to the screen space effect.
	 */
	ScreenSpaceEffect* addScreenSpaceEffect(ScreenSpaceEffectPointer screenSpaceEffect);

	/**
	 * Get the index of a vertex array object which holds information to draw a square.
	 */
	unsigned int getSquareVAO();

	/**
	 * Get the index of a vertex array object which holds information to draw a screen overlay rectangle.
	 */
	unsigned int getScreenRectVAO();

	/**
	 * Set the quality level of the MSAA.
	 * 
	 * \param qualityLevel: the new MSAA quality level.
	 */
	void setMSAAQualityLevel(unsigned int qualityLevel);

	// Functions related to bloom
	namespace Bloom
	{
		/**
		 * Set whether the bloom effect should be enabled.
		 * 
		 * \param enabled: whether the bloom effect should be enabled.
		 */
		void setEnabled(bool enabled);

		/**
		 * Set the bloom threshold (default = 0.7).
		 * Pixels with a brightness value above this threshold will be used in the bloom calculation.
		 *
		 * \param threshold: the new bloom threshold.
		 */
		void setThreshold(float threshold);

		/**
		 * Set the bloom intensity (default = 1).
		 *
		 * \param intensity: the new bloom intensity.
		 */
		void setIntensity(float intensity);

		/**
		 * Set the bloom radius.
		 * Not currently implemented.
		 *
		 * \param radius: the new bloom radius.
		 */
		void setRadius(int radius);
	}
};
