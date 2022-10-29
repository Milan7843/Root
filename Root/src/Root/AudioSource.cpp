#include "AudioSource.h"

#include <Root/engine/AudioEngine.h>

AudioSource::AudioSource()
{
	alGenSources(1, &sourceID);
	alSource3f(sourceID, AL_POSITION, 1.f, 0.f, 0.f);
	alSource3f(sourceID, AL_VELOCITY, 0.f, 0.f, 0.f);
	alSourcef(sourceID, AL_PITCH, 1.f);
	alSourcef(sourceID, AL_GAIN, 1.f);
	alSourcei(sourceID, AL_LOOPING, AL_FALSE);
}

AudioSource::AudioSource(const std::string& name)
{
	alGenSources(1, &sourceID);
	alSource3f(sourceID, AL_POSITION, 0.f, 0.f, 0.f);
	alSource3f(sourceID, AL_VELOCITY, 0.f, 0.f, 0.f);
	alSourcef(sourceID, AL_PITCH, 1.f);
	alSourcef(sourceID, AL_GAIN, 1.f);
	alSourcei(sourceID, AL_LOOPING, AL_FALSE);

	setAudioClip(name);
}

AudioSource::~AudioSource()
{
	alDeleteSources(1, &sourceID);
}

void AudioSource::play()
{
	alSourcePlay(sourceID);
}

void AudioSource::pause()
{
	alSourcePause(sourceID);
}

void AudioSource::setAudioClip(const std::string& name)
{
	unsigned int buffer{ AudioEngine::findBufferByName(name) };
	alSourcei(sourceID, AL_BUFFER, buffer);
}

void AudioSource::setGain(float gain)
{
	alSourcef(sourceID, AL_GAIN, gain);
}

void AudioSource::setPitch(float pitch)
{
	alSourcef(sourceID, AL_PITCH, pitch);
}

void AudioSource::setLooping(bool loop)
{
	alSourcei(sourceID, AL_LOOPING, loop);
}
