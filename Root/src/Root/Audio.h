#pragma once

#include <iostream>
#include <string>

namespace Audio
{
	/**
	 * Load a specific loaded sound.
	 * The sound will be saved under the given name, which it can be identified by later.
	 */
	void loadSound(const std::string& path, const std::string& name);

	/**
	 * Play a specific loaded sound.
	 * The sound must already be loaded.
	 */
	void playSound(const std::string& name);
};
