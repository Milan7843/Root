#pragma once

#include <Root/Logger.h>
#include <Root/CollisionListener.h>

#include <Root/components/Rigidbody.h>

/**
 * Class that handles the callbacks for the CollisionListener.
 */
class CollisionCallbackHandler : public b2ContactListener
{
public:

	CollisionCallbackHandler() {}
	~CollisionCallbackHandler() {}

	void BeginContact(b2Contact* contact) override;

	void EndContact(b2Contact* contact) override;
	
	void setCollisionListener(CollisionListener* contactListener);

private:
	CollisionListener* contactListener{ nullptr };
};

