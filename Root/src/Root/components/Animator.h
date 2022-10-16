#pragma once

#include <Root/animation/Animation.h>

#include "Root/Logger.h"

#include <memory>
#include <string>
#include <map>

struct BoolAnimationCondition
{
	std::string parameterTag{ "nc" }; // Start out as no-condition
	bool comparative;
};

struct AnimationLink
{
	std::string tag1;
	std::string tag2;
	BoolAnimationCondition condition;
};

class Animator
{
public:

	/**
	 * Create a new Animator.
	 */
	Animator();

	~Animator();

	void update();

	void addAnimation(Animation& animation, const std::string& tag);

	/**
	 * Create a link between the animations with tag1 and tag2 respectively.
	 * 
	 * \param tag1: the tag of the first animation.
	 * \param tag2: the tag of the second animation.
	 */
	void createLink(const std::string& tag1, const std::string& tag2);

	void addConditionToLink(const std::string& tag1, const std::string& tag2, const std::string& parameterTag, bool comparative);

	void setBool(const std::string& tag, bool value);

private:

	Animation* currentAnimation{ nullptr };
	std::string currentAnimationTag{};

	AnimationLink* getLinkByTags(const std::string& tag1, const std::string& tag2);

	bool evaluateCondition(BoolAnimationCondition& condition);

	std::map<std::string, Animation> animations;
	std::vector<AnimationLink> links;
	std::map<std::string, bool> boolParameters;
};

