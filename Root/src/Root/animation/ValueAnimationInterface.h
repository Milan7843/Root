#pragma once

/**
 * Interface for ValueAnimations, since they cannot go into a vector, as it is a template class.
 * The update function inside it will be overriden by the actual ValueAnimation
 */
class ValueAnimationInterface
{
public:
	/**
	 * Update the value of this animation.
	 * Not to be called by the user.
	 * 
	 * \returns whether the value animation is done.
	 */
	virtual bool update(float animationTime) { return false; }
};

