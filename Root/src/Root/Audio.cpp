#include "Audio.h"

#include <Root/engine/AudioEngine.h>

namespace Audio
{
	void loadSound(const std::string& path, const std::string& name)
	{
		AudioEngine::loadSound(path, name);
	}

	void playSound(const std::string& name)
	{
		AudioEngine::playSound(name);
	}
};



