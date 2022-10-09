#pragma once

#include "Root/engine/RootEngine.h"

#include <Root/CollisionListener.h>

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

	void destroyBody(b2Body* bodyToDestroy);
};


