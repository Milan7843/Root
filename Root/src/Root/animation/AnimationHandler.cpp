#include "AnimationHandler.h"

#include <Root/engine/AnimationHandlerEngine.h>

namespace AnimationHandler
{
	void addAnimator(Animator& animator, const std::string& tag)
	{
		AnimationHandlerEngine::addAnimator(animator, tag);
	}

	void setAnimatorParameter(const std::string& animatorTag, const std::string& parameterTag, bool value)
	{
		AnimationHandlerEngine::setAnimatorParameter(animatorTag, parameterTag, value);
	}
}
