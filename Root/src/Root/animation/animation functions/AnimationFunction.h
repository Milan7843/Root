#pragma once

#include <Root/animation/animation functions/AnimationFunctionCall.h>

/**
 * A class that holds a function to be called in an animation.
 */
class AnimationFunction : public AnimationFunctionCall
{
public:

	/**
	 * Create a new animation function.
	 * This is a function that does not belong to a class and will be called during an animation.
	 * The function must not belong to a class, or be static, and it must not have any parameters.
	 * To use a function that is attached to a class, see the ClassAnimationFunction class.
	 * To use a function with a parameter that is attached to a class,
	 * see the ClassParameterizedAnimationFunction class.
	 * 
	 * \param functionPointer: a pointer to the function to be called.
	 * \returns a pointer to the AnimationFunction that was created.
	 */
	static AnimationFunctionCallPointer create(void (*functionPointer)());
	~AnimationFunction();

	void call() override;


	void (*functionPointer)() = nullptr;

private:

	AnimationFunction(void (*functionPointer)());

};

