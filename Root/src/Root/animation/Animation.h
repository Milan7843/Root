#pragma once

#include <Root/animation/ValueAnimationInterface.h>
#include <Root/Time.h>
#include <Root/Logger.h>

#include <Root/animation/animation functions/AnimationFunctionCall.h>

#include <memory>
#include <vector>
#include <sstream>

enum class OnAnimationFinish
{
	STOP,
	REPEAT
};

struct AnimationFunctionAtTime
{
	std::shared_ptr<AnimationFunctionCall> animationFunction;
	float time;
	bool called{ false };
};

class Animation
{
public:

	/**
	 * Create a new animation.
	 */
	Animation();

	~Animation();

	/**
	 * Update the value animations of this animation.
	 * 
	 * \returns whether the animation was done.
	 */
	bool update();

	/**
	 * Play this animation.
	 */
	void play();

	/**
	 * Stop this animation.
	 */
	void stop();

	void addValueAnimation(std::shared_ptr<ValueAnimationInterface> valueAnimation);

	/**
	 * Set the behaviour of the animation finishing.
	 *
	 * \param onAnimationFinish: what happens whenthe animation finishes.
	 */
	void setOnFinish(OnAnimationFinish onAnimationFinish);

	/**
	 * Add an animation function.
	 * This will cause the animation function's function to be called at time point time.
	 * 
	 * /param animationFunction: the function to call at time time.
	 * /param time: the time at which to call the function.
	 */
	void addAnimationFunction(AnimationFunctionCallPointer& animationFunctionPointer, float time);

	/**
	 * Create a human readable string from the animation functions in this animation.
	 *
	 * \return the animation functions in this animation in a human readable format.
	 */
	std::string functionsToString();

private:

	std::vector<std::shared_ptr<ValueAnimationInterface>> valueAnimations;

	std::vector<AnimationFunctionAtTime> animationFunctions;

	// Defines what should happen when the animation finishes
	OnAnimationFinish onAnimationFinish { OnAnimationFinish::REPEAT };

	bool playing{ true };

	// The speed multiplier of this animation
	float animationSpeed{ 1.0f };

	// How long the animation has been going for
	float animationTime{ 0.0f };
};

