#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <string>

enum class OnFinishAudio
{
	PAUSE,
	LOOP,
	DESTROY_SELF
};

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
	 * Stop this audio source.
	 */
	void stop();

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
	 * Get whether this audio source is done.
	 * Only happens when the audio clip has finished playing
	 * and the onFinishAudio is set to DESTROY_SELF.
	 * 
	 * \return whether this audio source is done.
	 */
	bool isDone();

	/**
	 * Define what should happen when the audio playing finishes.
	 * 
	 * \param onFinishAudio: what should happen when the audio playing finishes.
	 */
	void setOnFinishAudio(OnFinishAudio onFinishAudio);

private:

	OnFinishAudio onFinishAudio{ OnFinishAudio::PAUSE };

	unsigned int sourceID{ 0 };
};
