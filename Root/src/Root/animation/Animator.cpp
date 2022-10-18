#include "Animator.h"

Animator::Animator()
{
}

Animator::~Animator()
{
	Logger::destructorMessage("Animator");
}

void Animator::update()
{
	if (animationsMapChanged)
	{
		// Re-get the current animation address
		if (currentAnimationTag != "")
		{
			currentAnimation = &animations[currentAnimationTag];
			animationsMapChanged = false;
		}
		else
		{
			Logger::logError("No animation set as start animation.");
		}
	}

	bool animationDone{ currentAnimation == nullptr ? true : currentAnimation->update() };

	// Going to the next animation:
	for (AnimationLink& link : links)
	{
		// Making sure the link starts from the current animation
		if (link.tag1 != currentAnimationTag)
			continue;

		// Check whether we must wait for the end of the previous animation
		if (link.waitForEndOfAnimation && !animationDone)
			continue;

		// The condition was true
		if (evaluateCondition(link.condition))
		{
			// Moving to the next animation
			currentAnimation = &animations[link.tag2];
			currentAnimationTag = link.tag2;
		}
	}
}

void Animator::addAnimation(Animation& animation, const std::string& tag, bool startAnimation)
{
	animations.emplace(tag, animation);
	
	animationsMapChanged = true;
	
	if (startAnimation)
	{
		//currentAnimation = &animations[tag];
		currentAnimationTag = tag;
		Logger::log(tag);
	}
	else
	{
		currentAnimation = nullptr;
	}
}

void Animator::createLink(const std::string& tag1, const std::string& tag2, bool waitForEndOfAnimation)
{
	links.emplace_back(AnimationLink{ tag1, tag2, waitForEndOfAnimation });
}

void Animator::addConditionToLink(const std::string& tag1, const std::string& tag2, const std::string& parameterTag, bool comparative)
{
	AnimationLink* link{ getLinkByTags(tag1, tag2) };

	link->condition = BoolAnimationCondition{ parameterTag, comparative };
}

void Animator::setBool(const std::string& tag, bool value)
{
	boolParameters[tag] = value;
}

void Animator::addParameter(const std::string& tag, bool initialValue)
{
	boolParameters.emplace(tag, initialValue);
}

AnimationLink* Animator::getLinkByTags(const std::string& tag1, const std::string& tag2)
{
	for (AnimationLink& link : links)
	{
		if (link.tag1 == tag1
			&& link.tag2 == tag2)
		{
			return &link;
		}
	}

	return nullptr;
}

bool Animator::evaluateCondition(BoolAnimationCondition& condition)
{
	if (condition.parameterTag == "nc")
	{
		return true;
	}

	bool parameterValue{ boolParameters[condition.parameterTag] };

	return condition.comparative == parameterValue;
}
