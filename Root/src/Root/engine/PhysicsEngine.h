#pragma once

#include "Root/engine/RootEngine.h"

#include <Root/CollisionCallbackHandler.h>

#include <box2d/box2d.h>

#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>

namespace PhysicsEngine
{
	void initialise();

	void simulate();

	void step(float deltaTima);

	void setGravity(float x, float y);

	void setCollisionListener(CollisionListener* contactListener);

	b2Body* addBody(b2BodyDef* definition);

	/**
	 * Destroy a particular body.
	 * 
	 * \returns true if the body was immediately destroyed,
	 * or false if the body was put into queue for destruction.
	 */
	bool destroyBody(b2Body* bodyToDestroy);
};


