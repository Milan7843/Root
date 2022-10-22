#pragma once

#include <Root/animation/AnimationWeb.h>

#include <vector>

namespace AnimationHandlerEngine
{
	struct TaggedAnimationWeb
	{
		AnimationWeb animationWeb;
		std::string tag;
	};

	void addAnimationWeb(AnimationWeb animationWeb, const std::string& tag);

	bool removeAnimationWeb(const std::string& tag);

	void setAnimationWebParameter(const std::string& animationWebTag, const std::string& parameterTag, bool value);

	/**
	 * Update all active animations webs.
	 */
	void update();

};
