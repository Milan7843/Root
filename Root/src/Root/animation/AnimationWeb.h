#pragma once

#include <Root/animation/Animation.h>

#include "Root/Logger.h"

#include <memory>
#include <string>
#include <sstream>
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
	bool waitForEndOfAnimation;
	BoolAnimationCondition condition;
};

/**
 * The Animation Web is a class which holds some animations, and any links between them.
 */
class AnimationWeb
{
public:

	/**
	 * Create a new Animation Web.
	 */
	AnimationWeb();

	~AnimationWeb();

	void update();

	/**
	 * Add an animation.
	 * 
	 * \param animation: the animation to add to the animator.
	 * \param tag: the tag the animation should be identified by.
	 * \param startAnimation: 
	 *		whether this animation should be the initial active animation (default = false)
	 */
	void addAnimation(Animation& animation, const std::string& tag, bool startAnimation = false);

	/**
	 * Create a link between the animations with tag1 and tag2 respectively.
	 * 
	 * \param tag1: the tag of the first animation.
	 * \param tag2: the tag of the second animation.
	 * \param waitForEndOfAnimation: 
	 * whether to wait for the end of the current animation to transition.
	 */
	void createLink(const std::string& tag1, const std::string& tag2, bool waitForEndOfAnimation);

	/**
	 * Add a condition to an existing link.
	 * 
	 * \param tag1: the tag of the "from" part of the link.
	 * \param tag2: the tag of the "to" part of the link.
	 * \param parameterTag: the tag of the parameter to check.
	 * \param comparative: what to compare the parameter to.
	 */
	void addConditionToLink(const std::string& tag1, const std::string& tag2, const std::string& parameterTag, bool comparative);

	/**
	 * Set a boolean parameter.
	 * 
	 * \param tag: the tag with which to identify the parameter.
	 * \param value: the new value of the parameter.
	 */
	void setBool(const std::string& tag, bool value);

	/**
	 * Add a parameter to this animator.
	 * 
	 * \param tag: the tag the parameter will be identified by.
	 * \param initialValue: the initial value of the parameter.
	 */
	void addParameter(const std::string& tag, bool initialValue);

	/**
	 * Create a human readable string from this animation web.
	 * 
	 * \return this animation web in a human readable format.
	 */
	std::string toString();

private:

	Animation* currentAnimation{ nullptr };
	std::string currentAnimationTag{};

	AnimationLink* getLinkByTags(const std::string& tag1, const std::string& tag2);

	std::vector<AnimationLink> getLinksByAnimationTag(const std::string& tag);

	bool evaluateCondition(BoolAnimationCondition& condition);

	// Indicates that the map which contains the animations may have changed,
	// and so indicates invalidation of the current pointer to the current animation.
	bool animationsMapChanged{ true };

	std::map<std::string, Animation> animations;
	std::vector<AnimationLink> links;
	std::map<std::string, bool> boolParameters;
};

