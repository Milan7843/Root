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

	/**
	 * Create a new class parameterized animation function.
	 * This is a function that belongs to a class and will be called during an animation.
	 * The function must belong to a class of type T and must have one parameter of type U.
	 * To use a function that is not attached to a class, or is static,
	 * see the AnimationFunction class.
	 * To use a function without a parameter that is attached to a class,
	 * see the ClassAnimationFunction class.
	 *
	 * \param instance: a pointer to the instance of type T to call the function on.
	 * \param functionPointer: a pointer to the function to be called.
	 * \param value: the parameter to pass when calling the function.
	 * \returns a pointer to the ClassParameterizedAnimationFunction that was created.
	 */
	static AnimationFunctionCallPointer create(T* instance, void (T::* functionPointer)(U), U& value);

	~ClassParameterizedAnimationFunction();

	void call() override;

private:

	ClassParameterizedAnimationFunction(T* instance, void (T::* functionPointer)(U), U& value);

	T* instance;

	U value;

	void (T::* functionPointer)() = nullptr;
};

template <class T, class U>
inline ClassParameterizedAnimationFunction<T, U>::ClassParameterizedAnimationFunction(T* instance, void (T::* functionPointer)(U), U& value)
	: instance(instance)
	, functionPointer(functionPointer)
	, value(value)
{
}

template<class T, class U>
inline AnimationFunctionCallPointer ClassParameterizedAnimationFunction<T, U>::create(T* instance, void(T::* functionPointer)(U), U& value)
{
	ClassParameterizedAnimationFunction<T, U>* classParameterizedAnimationFunction
		= new ClassParameterizedAnimationFunction<T, U>(instance, functionPointer, value);
	AnimationFunctionCallPointer pointer{ classParameterizedAnimationFunction };
	return pointer;
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
