#pragma once

#include "Root/components/Component.h" // Base class
#include <Root/animation/AnimationWeb.h>
#include <Root/animation/AnimationHandler.h>
#include <Root/animation/SetterValueAnimation.h>
#include <Root/animation/ValueAnimation.h>

#include <memory>

#define AnimatorPointer Animator*

class Transform;

// TODO change this to somehow only be in Transform.h
#define TransformPointer Transform*

class Animator : public Component
{
public:

	~Animator();

protected:

	/**
	 * Create a new animator.
	 * Will automatically add this component to the given transform.
	 *
	 * \param transform:		the transform to add this component to.
	 * \param animationWebTag:	the tag of the animator.
	 */
	static AnimatorPointer createBase(
		TransformPointer transform,
		std::string animationWebTag);

	Animator(std::string animationWebTag);

	std::string animationWebTag;
};
