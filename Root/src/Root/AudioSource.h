#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <string>

class AudioSource
{
public:
	AudioSource();

	/**
	 * Create a new audio source.
	 *
	 * \param name: the name of the audio clip that this audio source will play.
	 */
	AudioSource(const std::string& name);
	~AudioSource();

	/**
	 * Play this audio source.
	 */
	void play();

	/**
	 * Pause this audio source.
	 */
	void pause();

	/**
	 * Set the audio clip that this source plays.
	 * 
	 * \param name: the name of the audio clip that this audio source will play.
	 */
	void setAudioClip(const std::string& name);

	/**
	 * Set the gain of this audio source.
	 * 
	 * \param gain: the new gain. The gain is 1.0 by default.
	 */
	void setGain(float gain);

	/**
	 * Set the pitch of this audio source.
	 *
	 * \param pitch: the new pitch. The pitch is 1.0 by default.
	 */
	void setPitch(float pitch);

	/**
	 * Set whether this audio source should loop.
	 *
	 * \param loop: whether this audio source should loop.
	 */
	void setLooping(bool loop);

private:

	unsigned int sourceID{ 0 };
};
