#pragma once

#include <Root/animation/ValueAnimationInterface.h>
#include <Root/Time.h>
#include <Root/Logger.h>

#include <memory>
#include <vector>

enum class OnAnimationFinish
{
	STOP,
	REPEAT
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
	 */
	void update();

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


private:

	std::vector<std::shared_ptr<ValueAnimationInterface>> valueAnimations;

	// Defines what should happen when the animation finishes
	OnAnimationFinish onAnimationFinish;

	bool playing{ false };

	// The speed multiplier of this animation
	float animationSpeed{ 1.0f };

	// How long the animation has been going for
	float animationTime{ 0.0f };
};

