#include "Animator.h"

#include <Root/Transform.h>

Animator::~Animator()
{
	AnimationHandler::removeAnimationWeb(animationWebTag);
}

AnimatorPointer Animator::createBase(TransformPointer transform, std::string animationWebTag)
{
	Animator* animator = new Animator(animationWebTag);
	std::shared_ptr<Animator> pointer{ animator };
	transform->addComponent(pointer);
	return animator;
}

Animator::Animator(std::string animationWebTag)
	: animationWebTag(animationWebTag)
{
}
