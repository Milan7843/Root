#include "Animation.h"

Animation::Animation()
{
}

Animation::~Animation()
{
	Logger::destructorMessage("Animation");
}

bool Animation::update()
{
	// Don't update anything if it's not playing the animation
	if (!playing)
		return true;

	animationTime += Time::getDeltaTime() * animationSpeed;

	bool animationFinished{ true };
	for (std::shared_ptr<ValueAnimationInterface>& valueAnimation : valueAnimations)
	{
		// If any animation is not yet finished, the full animation cannot be finished
		if (!valueAnimation->update(animationTime))
			animationFinished = false;
	}

	std::cout << "AT ANIMATION FUNCTION CHECK: " << animationFunctions.size() << std::endl;

	for (AnimationFunctionAtTime& animationFunctionAtTime : animationFunctions)
	{
		// If it's already called no need to do it again
		if (animationFunctionAtTime.called)
			continue;

		// Check if it's time has passed, if it has call the function
		if (animationTime > animationFunctionAtTime.time)
		{
			animationFunctionAtTime.animationFunction.call();
			animationFunctionAtTime.called = true;
		}
	}


	// Check whether the animation is finished
	if (animationFinished)
	{
		switch (onAnimationFinish)
		{
			case OnAnimationFinish::STOP:
				stop();
				break;

			case OnAnimationFinish::REPEAT:
				play();
				break;
		}
	}

	return animationFinished;
}

void Animation::play()
{
	playing = true;
	animationTime = 0.0f;

	// Resetting all animation function call flags
	for (AnimationFunctionAtTime& animationFunctionAtTime : animationFunctions)
	{
		animationFunctionAtTime.called = false;
	}
}

void Animation::stop()
{
	this->playing = false;
}

void Animation::addValueAnimation(std::shared_ptr<ValueAnimationInterface> valueAnimation)
{
	valueAnimations.push_back(valueAnimation);
}

void Animation::setOnFinish(OnAnimationFinish onAnimationFinish)
{
	this->onAnimationFinish = onAnimationFinish;
}

void Animation::addAnimationFunction(AnimationFunctionCall& animationFunction, float time)
{
	animationFunctions.push_back(AnimationFunctionAtTime{ animationFunction, time });
	std::cout << "ADDING ANIMATION FUNCTION" << std::endl;
}

std::string Animation::functionsToString()
{
	std::stringstream sstream;

	if (animationFunctions.size() == 0)
		return sstream.str();

	sstream << "Functions: " << "\n";

	for (AnimationFunctionAtTime& animationFunctionAtTime : animationFunctions)
	{
		sstream << "           ";

		sstream << "Animation function at t: " << animationFunctionAtTime.time << "\n";
	}

	return sstream.str();
}
