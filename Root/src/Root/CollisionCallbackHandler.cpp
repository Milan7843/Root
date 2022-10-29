#include "CollisionCallbackHandler.h"

#include <Root/Transform.h>

void CollisionCallbackHandler::BeginContact(b2Contact* contact)
{
	if (contactListener == nullptr)
		return;

	// Retrieving both rigidbodies
	FixtureData* fixtureData1{ reinterpret_cast<FixtureData*>(contact->GetFixtureA()->GetUserData().pointer) };
	FixtureData* fixtureData2{ reinterpret_cast<FixtureData*>(contact->GetFixtureB()->GetUserData().pointer) };

	// Either one of the rigidbodies is destroyed
	if (fixtureData1 == nullptr || fixtureData2 == nullptr)
		return;

	Rigidbody* rb1{ fixtureData1->rigidbody };
	Rigidbody* rb2{ fixtureData2->rigidbody };

	// Either one of the rigidbodies is destroyed
	if (rb1 == nullptr || rb2 == nullptr || rb1->getTransform() == nullptr || rb2->getTransform() == nullptr)
		return;

	// Check for tag order:
	// compare() returns a negative value if the first char that doesn't match is less
	if (rb2->getTransform()->getTag().compare(rb1->getTransform()->getTag()) < 0)
	{
		// Swapping the order if the tags are 
		Rigidbody* temp = rb2;
		rb2 = rb1;
		rb1 = temp;
	}

	// Call the user-defined callback
	contactListener->BeginContact(contact, rb1, rb2);
}

void CollisionCallbackHandler::EndContact(b2Contact* contact)
{
	if (contactListener == nullptr)
		return;

	// Retrieving both rigidbodies
	FixtureData* fixtureData1{ reinterpret_cast<FixtureData*>(contact->GetFixtureA()->GetUserData().pointer) };
	FixtureData* fixtureData2{ reinterpret_cast<FixtureData*>(contact->GetFixtureB()->GetUserData().pointer) };

	// Either one of the rigidbodies is destroyed
	if (fixtureData1 == nullptr || fixtureData2 == nullptr)
		return;

	Rigidbody* rb1{ fixtureData1->rigidbody };
	Rigidbody* rb2{ fixtureData2->rigidbody };

	// Either one of the rigidbodies is destroyed
	if (rb1 == nullptr || rb2 == nullptr || rb1->getTransform() == nullptr || rb2->getTransform() == nullptr)
		return;

	// Check for tag order:
	// compare() returns a negative value if the first char that doesn't match is less
	if (rb2->getTransform()->getTag().compare(rb1->getTransform()->getTag()) < 0)
	{
		// Swapping the order if the tags are 
		Rigidbody* temp = rb2;
		rb2 = rb1;
		rb1 = temp;
	}

	// Call the user-defined callback
	contactListener->EndContact(contact, rb1, rb2);
}

void CollisionCallbackHandler::setCollisionListener(CollisionListener* contactListener)
{
	this->contactListener = contactListener;
}
