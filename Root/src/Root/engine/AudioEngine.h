#pragma once

#include <Root/Logger.h>

#include <AL/al.h>
#include <AL/alc.h>

#include <AudioFile/AudioFile.h>

#include <iostream>

#include <map>
#include <vector>

namespace AudioEngine
{
	void initialise();

	void terminate();

	void loadSound(const std::string& path, const std::string& name);

	void playSound(const std::string& name);

	unsigned int findBufferByName(const std::string& name);
};
