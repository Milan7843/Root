#pragma once

#include <Root/components/Rigidbody.h>

#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_body.h>
#include <box2d/b2_settings.h>

/**
 * A class that should be the base class of your collision callback handler.
 * In order to use it:
 * - Create a subclass of this class
 * - Implement BeginContact() and EndContact() (optional)
 * - Create an instance of your new class
 * - Use Physics::setCollisionListener() with a pointer to the new instance
 * - Make sure the instance stays alive
 */
class CollisionListener
{
public:

	virtual void BeginContact(b2Contact* contact, Rigidbody* rb1, Rigidbody* rb2) {}

	virtual void EndContact(b2Contact* contact, Rigidbody* rb1, Rigidbody* rb2) {}
};

