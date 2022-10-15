#pragma once

#include <Root/animation/ValueAnimationInterface.h>
#include <Root/animation/Animation.h>

#include <Root/Gradient.h>

#define ValueAnimationPointer ValueAnimation<T>*

template <class T>
class ValueAnimation : public ValueAnimationInterface
{
public:
	/**
	 * Create a new animation.
	 *
	 * \param animation: what animation this value animation should be added to.
	 * \param valuePointer: a pointer to the value that will actually be changed.
	 * \param gradient: the gradient that decides the values that the value will take.
	 */
	static ValueAnimation<T>* create(Animation* animation, T* valuePointer, Gradient<T> gradient);

	~ValueAnimation();

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
	 * \param valuePointer: a pointer to the value that will actually be changed.
	 * \param gradient: the gradient that decides the values that the value will take.
	 */
	ValueAnimation(T* valuePointer, Gradient<T> gradient);

	// The speed multiplier of this animation
	float animationSpeed{ 1.0f };

	// How long this animation takes
	float duration{ 1.0f };
	
	Gradient<T> gradient;

	T* value;
};

template<class T>
inline ValueAnimation<T>::ValueAnimation(T* valuePointer, Gradient<T> gradient)
	: value(valuePointer)
	, gradient(gradient)
{
}

template<class T>
inline ValueAnimation<T>* ValueAnimation<T>::create(Animation* animation, T* valuePointer, Gradient<T> gradient)
{
	ValueAnimation<T>* valueAnimation = new ValueAnimation<T>(valuePointer, gradient);
	std::shared_ptr<ValueAnimationInterface> pointer{ valueAnimation };
	animation->addValueAnimation(pointer);
	return valueAnimation;
}

template<class T>
inline ValueAnimation<T>::~ValueAnimation()
{
	Logger::destructorMessage("Value animation");
}

template<class T>
inline bool ValueAnimation<T>::update(float animationTime)
{
	float usingAnimationTime{ animationTime * animationSpeed };
	(*value) = gradient.sample(usingAnimationTime);

	// Return whether the animation is finished
	return (usingAnimationTime >= duration);
}

template<class T>
inline void ValueAnimation<T>::setValuePointer(T* valuePointer)
{
	this->value = valuePointer;
}

template<class T>
inline void ValueAnimation<T>::setGradient(Gradient<T> gradient)
{
	this->gradient = gradient;
}

template<class T>
inline void ValueAnimation<T>::setDuration(float duration)
{
	this->duration = duration;
}
