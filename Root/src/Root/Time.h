#pragma once

namespace Time
{
	/**
	 * Update Time with a new time to set its variables.
	 * 
	 * \param time: the current time
	 */
	void update(float time);

	/**
	 * Get the current time as the number of seconds since the beginning of the application.
	 *
	 * \returns the current time.
	 */
	float getTime();

	/**
	 * Get the current deltaTime, which is the number of seconds since the last frame.
	 *
	 * \returns the time difference since the last frame.
	 */
	float getDeltaTime();

	/**
	 * Set the current time scale.
	 *
	 * \param newTimeScale: the value the time scale will be set to.
	 */
	void setTimeScale(float newTimeScale);

	/**
	 * Get the current time scale.
	 *
	 * \returns the current time scale
	 */
	float getTimeScale();
};
