#pragma once

#include <Root/animation/animation functions/AnimationFunctionCall.h>

/**
 * A class that holds a function to be called in an animation.
 */
class AnimationFunction : public AnimationFunctionCall
{
public:

	AnimationFunction(void (* functionPointer)());
	~AnimationFunction();

	void call() override;


	void (*functionPointer)() = nullptr;

private:

};

