#pragma once

#include <Root/animation/Animator.h>

namespace AnimationHandler
{
	/**
	 * Add an animator to the handler.
	 * 
	 * \param animator: the animator to add.
	 * Will be copied, so the original instance will be active.
	 * \param tag: the tag to assign to the animator, which it will be identified by in the future.
	 */
	void addAnimator(Animator& animator, const std::string& tag);

	/**
	 * Set a bool parameter in a certain animator.
	 * 
	 * \param animatorTag: the tag of the animator to set the parameter in.
	 * \param parameterTag: the tag of the parameter to set.
	 * \param value: the new value of the parameter.
	 */
	void setAnimatorParameter(const std::string& animatorTag, const std::string& parameterTag, bool value);
};
