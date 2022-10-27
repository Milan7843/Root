#pragma once

#include <box2d/box2d.h>
#include <Root/CollisionListener.h>

namespace Physics
{
	/**
	 * Set a new collision listener as the active collision listener.
	 */
	void setCollisionListener(CollisionListener* collisionListener);

	/**
	 * Set the general gravity.
	 * 
	 * \param x: the x-component of the gravity (side to side)
	 * \param y: the y-component of the gravity (up or down)
	 */
	void setGravity(float x, float y);
};

