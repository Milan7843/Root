#pragma once

#include <Root/animation/ValueAnimationInterface.h>
#include <Root/animation/Animation.h>

#include <Root/Gradient.h>

#include <functional> // std::bind

#define ValueAnimationPointer ValueAnimation<T>*

template <class T, class U>
// T = type of value to be changed
// U = type of instance whose setter is called
class SetterValueAnimation : public ValueAnimationInterface
{
public:
	 /**
	  * Create a new setter value animation.
	  *
	  * \param animation: what animation this value animation should be added to.
	  * \param functionPointer: a pointer to the setter function that will be used to update the value.
	  * \param gradient: the gradient that decides the values that the value will take.
	  */
	static SetterValueAnimation<T, U>* create(Animation* animation, U* instance, void (U::* functionPointer)(T), Gradient<T> gradient);

	~SetterValueAnimation();

	/**
	 * Update the value of this animation.
	 * Not to be called by the user.
	 */
	virtual bool update(float animationTime) override;

	/**
	 * Set the pointer to the value that will be changed by the animation.
	 *
	 * \param valuePointer: a pointer to the value that will be changed by the animation.
	 */
	void setValuePointer(T* valuePointer);

	/**
	 * Set the gradient that decides the values that the value will take.
	 *
	 * \param gradient: the gradient that decides the values that the value will take.
	 */
	void setGradient(Gradient<T> gradient);

	/**
	 * Set the regular duration of this value animation.
	 *
	 * \param duration: the duration of this value animation.
	 */
	void setDuration(float duration);

private:
	/**
	 * Create a new animation.
	 *
	 * \param functionPointer: a pointer to the setter function that will be used to update the value.
	 * \param gradient: the gradient that decides the values that the value will take.
	 */
	SetterValueAnimation(U* instance, void (U::* functionPointer)(T), Gradient<T> gradient);

	// The speed multiplier of this animation
	float animationSpeed{ 1.0f };

	// How long this animation takes
	float duration{ 1.0f };

	Gradient<T> gradient;

	void (U::* functionPointer)(T) = nullptr;
	U* instance = nullptr;
};

template<class T, class U>
inline SetterValueAnimation<T, U>::SetterValueAnimation(U* instance, void (U::* functionPointer)(T), Gradient<T> gradient)
	: instance(instance)
	, functionPointer(functionPointer)
	, gradient(gradient)
{
}

template<class T, class U>
inline SetterValueAnimation<T, U>* SetterValueAnimation<T, U>::create(Animation* animation, U* instance, void (U::* functionPointer)(T), Gradient<T> gradient)
{
	SetterValueAnimation<T, U>* valueAnimation = new SetterValueAnimation<T, U>(instance, functionPointer, gradient);
	std::shared_ptr<ValueAnimationInterface> pointer{ valueAnimation };
	animation->addValueAnimation(pointer);
	return valueAnimation;
}

template<class T, class U>
inline SetterValueAnimation<T, U>::~SetterValueAnimation()
{
	Logger::destructorMessage("Value animation");
}

template<class T, class U>
inline bool SetterValueAnimation<T, U>::update(float animationTime)
{
	float usingAnimationTime{ animationTime * animationSpeed };

	T newValue = gradient.sample(usingAnimationTime);

	if (functionPointer != nullptr)
		(instance->*functionPointer)(newValue);

	// Return whether the animation is finished
	std::cout << "ANIMATION TIME: " << usingAnimationTime << "   DURATION: " << duration << std::endl;
	return (usingAnimationTime >= duration);
}

template<class T, class U>
inline void SetterValueAnimation<T, U>::setValuePointer(T* valuePointer)
{
	this->value = valuePointer;
}

template<class T, class U>
inline void SetterValueAnimation<T, U>::setGradient(Gradient<T> gradient)
{
	this->gradient = gradient;
}

template<class T, class U>
inline void SetterValueAnimation<T, U>::setDuration(float duration)
{
	this->duration = duration;
}
