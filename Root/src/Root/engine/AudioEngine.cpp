#include "AudioEngine.h"

#include <Root/AudioSource.h>

namespace AudioEngine
{
	namespace
	{
		std::map<std::string, unsigned int> soundBufferIDs;

		std::vector<std::shared_ptr<AudioSource>> activeAudioSources;

		ALCcontext* context{ nullptr };
		ALCdevice* device{ nullptr };

		ALenum convertFileToOpenALFormat(const AudioFile<float>& audioFile) {
			int bitDepth = audioFile.getBitDepth();
			if (bitDepth == 16)
				return audioFile.isStereo() ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			else if (bitDepth == 8)
				return audioFile.isStereo() ? AL_FORMAT_STEREO8 : AL_FORMAT_MONO8;
			else
				return -1; // this shouldn't happen!
		};
	}

	void initialise()
	{
		// Getting the default audio device
		const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
		device = alcOpenDevice(defaultDeviceString);
		if (!device)
		{
			Logger::logError("OpenAL: Failed to get default device.");
			return;
		}

		// Creating an audio context
		context = alcCreateContext(device, nullptr);
		if (!alcMakeContextCurrent(context))
		{
			Logger::logError("OpenAL: Failed to make context current.");
			return;
		}

		// Creating a listener
		alListener3f(AL_POSITION, 0, 0, 0);
		alListener3f(AL_VELOCITY, 0, 0, 0);
		ALfloat forwardAndUpVectors[] = {
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		};
		alListenerfv(AL_ORIENTATION, forwardAndUpVectors);
	}

	void terminate()
	{
		// Deleting all buffers
		for (std::pair<std::string, unsigned int> buffer : soundBufferIDs)
		{
			alDeleteBuffers(1, &buffer.second);
		}
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}

	void playSound(const std::string& name)
	{
		unsigned int buffer{ findBufferByName(name) };
		if (buffer == 0)
		{
			// No audio clip was found with the given name
			Logger::logError("Tried to play sound with name that was not loaded: " + name);
			return;
		}

		AudioSource* source = new AudioSource(name);
		source->play();
		std::shared_ptr<AudioSource> pointer(source);

		activeAudioSources.push_back(pointer);
	}

	void loadSound(const std::string& path, const std::string& name)
	{
		AudioFile<float> soundFile;

		if (!soundFile.load(path))
		{
			Logger::logError("AudioFile: Failed to load sound at " + path);
		}

		std::vector<uint8_t> PCMDataBytes;
		soundFile.writePCMToBuffer(PCMDataBytes);

		// Generating a buffer and putting the audio file into it
		ALuint bufferID;
		alGenBuffers(1, &bufferID);
		alBufferData(bufferID,
			convertFileToOpenALFormat(soundFile),
			PCMDataBytes.data(),
			PCMDataBytes.size(),
			soundFile.getSampleRate());

		// Saving the buffer ID
		soundBufferIDs.emplace(name, bufferID);
	}

	unsigned int findBufferByName(const std::string& name)
	{
		std::map<std::string, unsigned int>::iterator iterator = soundBufferIDs.find(name);

		if (iterator != soundBufferIDs.end())
			return iterator->second;

		return 0;
	}
}; 