#pragma once

#include <Root/animation/animation functions/AnimationFunctionCall.h>

template <class T>
/**
 * A class that holds a class function to be called in an animation.
 * Template types:
 * - T: the class to call the function on.
 */
class ClassAnimationFunction : public AnimationFunctionCall
{
public:

	ClassAnimationFunction(T* instance, void (T::* functionPointer)());
	~ClassAnimationFunction();

	void call() override;

private:

	T* instance;

	void (T::*functionPointer)() = nullptr;
};

template<class T>
inline ClassAnimationFunction<T>::ClassAnimationFunction(T* instance, void (T::* functionPointer)())
	: instance(instance)
	, functionPointer(functionPointer)
{
}

template<class T>
inline ClassAnimationFunction<T>::~ClassAnimationFunction()
{
}

template<class T>
inline void ClassAnimationFunction<T>::call()
{
	if (functionPointer == nullptr)
	{
		return;
	}

	if (instance == nullptr)
	{
		return;
	}

	std::cout << "CALLING" << std::endl;

	// Calling the function
	(instance->*functionPointer)();
}
