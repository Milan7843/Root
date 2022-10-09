#pragma once

#include <cstdlib>

namespace Random
{
	/**
	 * Set the seed for the random number generator.
	 * 
	 * \param seed: the new seed.
	 */
	void setSeed(unsigned int seed);

	/**
	 * Get a random number between min and max.
	 * 
	 * \param min: the minimum value of the returned random number.
	 * \param max: the maximum value of the returned random number.
	 * \returns a random number between min and max.
	 */
	float between(float min, float max);
};

