#include "Animation.h"

Animation::Animation()
{
}

Animation::~Animation()
{
	Logger::destructorMessage("Animation");
}

void Animation::update()
{
	// Don't update anything if it's not playing the animation
	if (!playing)
		return;

	animationTime += Time::getDeltaTime() * animationSpeed;

	bool animationFinished{ true };
	for (std::shared_ptr<ValueAnimationInterface>& valueAnimation : valueAnimations)
	{
		// If any animation is not yet finished, the full animation cannot be finished
		if (!valueAnimation->update(animationTime))
			animationFinished = false;
	}


	// Check whether the animation is finished
	if (animationFinished)
	{
		switch (onAnimationFinish)
		{
			case OnAnimationFinish::STOP:
				playing = false;
				break;

			case OnAnimationFinish::REPEAT:
				animationTime = 0.0f;
				break;
		}
	}
}

void Animation::play()
{
	playing = true;
	animationTime = 0.0f;
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
