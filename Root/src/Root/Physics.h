#pragma once

#include <box2d/box2d.h>
#include <Root/CollisionListener.h>

namespace Physics
{
	/**
	 * Set a new collision listener as the active collision listener.
	 */
	void setCollisionListener(CollisionListener* collisionListener);
};

