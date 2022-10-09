#include "Animator.h"

Animator::~Animator()
{
	Logger::destructorMessage("Animator");
}

AnimatorPointer Animator::create()
{
	return nullptr;
}
