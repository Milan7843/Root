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
	bool animationDone{ currentAnimation->update() };

	if (animationDone)
	{
		// Going to the next animation:
		for (AnimationLink& link : links)
		{
			// Making sure the link starts from the current animation
			if (link.tag1 != currentAnimationTag)
				continue;

			// The condition was true
			if (evaluateCondition(link.condition))
			{
				currentAnimation = &animations[link.tag2];
				currentAnimationTag = link.tag2;
			}
		}
	}
}

void Animator::addAnimation(Animation& animation, const std::string& tag)
{
	animations.emplace(tag, animation);
}

void Animator::createLink(const std::string& tag1, const std::string& tag2)
{
	links.emplace_back(AnimationLink{ tag1, tag2 });
}

void Animator::addConditionToLink(const std::string& tag1, const std::string& tag2, const std::string& parameterTag, bool comparative)
{
	AnimationLink* link{ getLinkByTags(tag1, tag2) };

	link->condition = BoolAnimationCondition{ parameterTag, comparative };
}

void Animator::setBool(const std::string& tag, bool value)
{
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
