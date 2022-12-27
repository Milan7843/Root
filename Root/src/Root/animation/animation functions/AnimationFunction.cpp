#include "AnimationFunction.h"

AnimationFunction::AnimationFunction(void (*functionPointer)())
	: functionPointer(functionPointer)
{
}

AnimationFunction::~AnimationFunction()
{
}

void AnimationFunction::call()
{
	if (functionPointer == nullptr)
	{
		return;
	}

	// Calling the function
	functionPointer();
}
