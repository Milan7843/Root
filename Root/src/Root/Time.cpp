#include "Time.h"

namespace Time
{
	namespace
	{
		float deltaTime{ 0.0f };
		float time{ 0.0f };
		float timeScale{ 1.0f };
	}

	void update(float newTime)
	{
		deltaTime = (newTime - time) * timeScale;
		time = newTime;
	}

	float getTime()
	{
		return time;
	}

	float getDeltaTime()
	{
		return deltaTime;
	}

	void setTimeScale(float newTimeScale)
	{
		timeScale = newTimeScale;
	}

	float getTimeScale()
	{
		return timeScale;
	}
}
