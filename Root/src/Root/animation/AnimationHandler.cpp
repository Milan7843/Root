#include "AnimationHandler.h"

#include <Root/engine/AnimationHandlerEngine.h>

namespace AnimationHandler
{
	void addAnimationWeb(AnimationWeb& animationWeb, const std::string& tag)
	{
		AnimationHandlerEngine::addAnimationWeb(animationWeb, tag);
	}

	bool removeAnimationWeb(const std::string& tag)
	{
		return AnimationHandlerEngine::removeAnimationWeb(tag);
	}

	void setAnimationWebParameter(const std::string& animationWebTag, const std::string& parameterTag, bool value)
	{
		AnimationHandlerEngine::setAnimationWebParameter(animationWebTag, parameterTag, value);
	}
}
