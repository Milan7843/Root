#pragma once

#include <Root/animation/animation functions/AnimationFunctionCall.h>

template <class T, class U>
/**
 * A class that holds a class function with a parameter to be called in an animation.
 * Template types:
 * - T: the class to call the function on.
 * - U: the type of the parameter.
 */
class ClassParameterizedAnimationFunction : public AnimationFunctionCall
{
public:

	ClassParameterizedAnimationFunction(T* instance, void (T::* functionPointer)(U), U value);
	~ClassParameterizedAnimationFunction();

	void call() override;

private:

	T* instance;

	U value;

	void (T::* functionPointer)() = nullptr;
};

template <class T, class U>
inline ClassParameterizedAnimationFunction<T, U>::ClassParameterizedAnimationFunction(T* instance, void (T::* functionPointer)(U), U value)
	: instance(instance)
	, functionPointer(functionPointer)
	, value(value)
{
}

template <class T, class U>
inline ClassParameterizedAnimationFunction<T, U>::~ClassParameterizedAnimationFunction()
{
}

template <class T, class U>
inline void ClassParameterizedAnimationFunction<T, U>::call()
{
	if (functionPointer == nullptr)
	{
		return;
	}

	if (instance == nullptr)
	{
		return;
	}

	// Calling the function
	instance->*functionPointer(value);
}
