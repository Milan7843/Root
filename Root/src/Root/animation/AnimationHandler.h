#pragma once

#include <Root/animation/AnimationWeb.h>

namespace AnimationHandler
{
	/**
	 * Add an animation web to the handler.
	 * 
	 * \param animationWeb: the animation web to add.
	 * Will be copied, so the original instance will not be active.
	 * \param tag: the tag to assign to the animation web, which it will be identified by in the future.
	 */
	void addAnimationWeb(AnimationWeb& animationWeb, const std::string& tag);

	/**
	 * Set a bool parameter in a certain animation web.
	 * 
	 * \param animatorTag: the tag of the animation web to set the parameter in.
	 * \param parameterTag: the tag of the parameter to set.
	 * \param value: the new value of the parameter.
	 */
	void setAnimationWebParameter(const std::string& animationWebTag, const std::string& parameterTag, bool value);
};
