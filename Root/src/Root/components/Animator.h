#pragma once

#include "Root/components/Component.h" // Base class
#include "Root/Logger.h"

#include <memory>

#define AnimatorPointer Animator*

class Animator : public Component
{
public:

	/**
	 * Create a new Animator component.
	 * Requires a SpriteRenderer component.
	 */
	Animator();
	~Animator();

	AnimatorPointer create();

private:


};

