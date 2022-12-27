#include "AnimationWeb.h"

AnimationWeb::AnimationWeb()
{
}

AnimationWeb::~AnimationWeb()
{
	Logger::destructorMessage("Animation web");
}

void AnimationWeb::update()
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

		// Check each condition
		bool allConditionsMet{ true };
		for (BoolAnimationCondition& condition : link.conditions)
		{
			if (!evaluateCondition(condition))
			{
				allConditionsMet = false;
				break;
			}
		}

		// Not all conditions were met, check the next link
		if (!allConditionsMet)
			continue;

		// All conditions were met, transition to the next animation

		// Moving to the next animation
		currentAnimation = &animations[link.tag2];
		currentAnimationTag = link.tag2;

		// Resetting all triggers
		for (BoolAnimationCondition& condition : link.conditions)
		{
			if (condition.conditionType == ConditionType::TRIGGER)
			{
				boolParameters[condition.parameterTag] = false;
			}
		}
	}
}

void AnimationWeb::addAnimation(Animation& animation, const std::string& tag, bool startAnimation)
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

void AnimationWeb::createLink(const std::string& tag1, const std::string& tag2, bool waitForEndOfAnimation)
{
	links.emplace_back(AnimationLink{ tag1, tag2, waitForEndOfAnimation });
}

void AnimationWeb::addConditionToLink(const std::string& tag1,
	const std::string& tag2,
	const std::string& parameterTag,
	ConditionType conditionType,
	bool comparative)
{
	AnimationLink* link{ getLinkByTags(tag1, tag2) };

	link->conditions.push_back(BoolAnimationCondition{ parameterTag, comparative, conditionType });
}

void AnimationWeb::setBool(const std::string& tag, bool value)
{
	//std::map<std::string, bool>::iterator iterator{ boolParameters.find(tag) };

	// Check if a parameter with the given tag exists
	if (boolParameters.find(tag) == boolParameters.end())
	{
		Logger::logError("Tried to access animation web with tag that doesn't exist. (" + tag + ")");
	}

	boolParameters[tag] = value;
	/*
	else
	{
		iterator->second = value;
	}*/
}

void AnimationWeb::addParameter(const std::string& tag, bool initialValue)
{
	boolParameters.emplace(tag, initialValue);
}

std::vector<AnimationLink> AnimationWeb::getLinksByAnimationTag(const std::string& tag)
{
	std::vector<AnimationLink> result;

	for (AnimationLink& link : links)
	{
		if (link.tag1 == tag)
		{
			result.push_back(link);
		}
	}

	return result;
}

std::string AnimationWeb::toString()
{
	std::stringstream sstream;

	sstream << "Animation web: \n";
	sstream << "Active animation: " << (currentAnimationTag == "" ? "None" : currentAnimationTag) << "\n";
	
	for (std::pair<std::string, Animation> taggedAnimation : animations)
	{
		const std::string& tag{ taggedAnimation.first };

		sstream << "Animation: \n";
		sstream << "Identifying tag: " << tag << "\n";
		sstream << "Links:\n";
		for (AnimationLink& link : getLinksByAnimationTag(tag))
		{
			sstream << "  - " << tag << " to " << link.tag2;

			unsigned int conditionIndex{ 0 };

			for (BoolAnimationCondition& condition : link.conditions)
			{

				if (conditionIndex == 0)
				{
					sstream << " if ";
				}
				else
				{
					sstream << " and ";
				}

				sstream << condition.parameterTag << " "
					<< ((condition.conditionType == ConditionType::BOOLEAN) ? "(boolean)" : "(trigger)")
					<< " is " << (condition.comparative ? "true" : "false");

				conditionIndex++;
			}

			sstream << "\n";
		}
		sstream << "\n";
	}

	return sstream.str();
}

AnimationLink* AnimationWeb::getLinkByTags(const std::string& tag1, const std::string& tag2)
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

bool AnimationWeb::evaluateCondition(BoolAnimationCondition& condition)
{
	bool parameterValue{ boolParameters[condition.parameterTag] };

	return condition.comparative == parameterValue;
}
