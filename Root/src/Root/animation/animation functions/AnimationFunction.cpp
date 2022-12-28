#include "AnimationFunction.h"

AnimationFunction::AnimationFunction(void (*functionPointer)())
	: functionPointer(functionPointer)
{
}

AnimationFunctionCallPointer AnimationFunction::create(void(*functionPointer)())
{
	AnimationFunction* animationFunction
		= new AnimationFunction(functionPointer);
	AnimationFunctionCallPointer pointer{ animationFunction };
	return pointer;
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
