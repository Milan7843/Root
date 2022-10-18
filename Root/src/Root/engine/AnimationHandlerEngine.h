#pragma once

#include <Root/animation/Animator.h>

#include <vector>

namespace AnimationHandlerEngine
{
	struct TaggedAnimator
	{
		Animator animator;
		const std::string tag;
	};

	void addAnimator(Animator animator, const std::string& tag);

	void setAnimatorParameter(const std::string& animatorTag, const std::string& parameterTag, bool value);

	/**
	 * Update all active animators.
	 */
	void update();

};
