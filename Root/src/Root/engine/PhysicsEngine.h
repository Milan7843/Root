#pragma once

#include "Root/engine/RootEngine.h"

#include <Root/CollisionCallbackHandler.h>

#include <box2d/box2d.h>

#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>

#include <queue>

namespace PhysicsEngine
{
	struct BodyEnabledStateChange
	{
		b2Body* body;
		bool flag;
	};

	void initialise();

	void simulate();

	void step(float deltaTima);

	void renderDebugView();

	void enableDebugMode();

	void disableDebugMode();

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

	/**
	 * Set a particular body to be enabled or disabled.
	 *
	 * \param body: the body for which to change the enabled state.
	 * \param flag: true for enabling, false for disabling.
	 * \returns true if the state of the body was immediately changed,
	 * or false if the body was put into queue for enabling/disabling.
	 */
	bool setBodyEnabled(b2Body* body, bool flag);
};


