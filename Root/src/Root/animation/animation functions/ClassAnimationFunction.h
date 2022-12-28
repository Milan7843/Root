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

	/**
	 * Create a new class animation function.
	 * This is a function that belongs to a class and will be called during an animation.
	 * The function must belong to a class of type T and must have no parameters.
	 * To use a function that is not attached to a class, or is static,
	 * see the AnimationFunction class.
	 * To use a function with a parameter that is attached to a class,
	 * see the ClassParameterizedAnimationFunction class.
	 *
	 * \param instance: a pointer to the instance of type T to call the function on.
	 * \param functionPointer: a pointer to the function to be called.
	 * \returns a pointer to the ClassAnimationFunction that was created.
	 */
	static AnimationFunctionCallPointer create(T* instance, void (T::* functionPointer)());

	ClassAnimationFunction(T* instance, void (T::* functionPointer)());
	~ClassAnimationFunction();

	void call() override;

private:

	T* instance;

	void (T::*functionPointer)() = nullptr;
};

template<class T>
inline AnimationFunctionCallPointer ClassAnimationFunction<T>::create(T* instance, void(T::* functionPointer)())
{
	ClassAnimationFunction<T>* classAnimationFunction
		= new ClassAnimationFunction<T>(instance, functionPointer);
	AnimationFunctionCallPointer pointer{ classAnimationFunction };
	return pointer;
}

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
	std::cout << "CALLING" << std::endl;

	if (functionPointer == nullptr)
	{
		return;
	}

	if (instance == nullptr)
	{
		return;
	}

	// Calling the function
	(instance->*functionPointer)();
}
