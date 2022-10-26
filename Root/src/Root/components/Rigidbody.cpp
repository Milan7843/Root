#include "Rigidbody.h"

#include "Root/engine/PhysicsEngine.h"

#include <box2d/b2_settings.h>

Rigidbody::Rigidbody(TransformPointer transform, float linearDamping, float angularDamping, bool allowSleep, bool awake, bool fixedRotation, bool bullet, b2BodyType type, bool enabled, float gravityScale)
{
	glm::vec2 position{ transform->getPosition() };

	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(position.x, position.y);
	bodyDef.angle = glm::radians(transform->getRotation());
	bodyDef.type = type;
	bodyDef.linearDamping = linearDamping;
	bodyDef.angularDamping = angularDamping;
	bodyDef.allowSleep = allowSleep;
	bodyDef.awake = awake;
	bodyDef.fixedRotation = fixedRotation;
	bodyDef.bullet = bullet;
	bodyDef.enabled = enabled;
	bodyDef.gravityScale = gravityScale;

	body = PhysicsEngine::addBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	// Creating a fixture data
	fixtureData = new FixtureData; 

	b2Fixture* fixture{ body->CreateFixture(&fixtureDef) };

	// Assigning the user data pointer of the fixture
	uintptr_t ptr = reinterpret_cast<uintptr_t>(fixtureData);
	fixture->GetUserDataRef().pointer = ptr;

	fixtureData->mFixture = fixture;
	fixtureData->rigidbody = this;
}

Rigidbody::Rigidbody(TransformPointer transform,
	Collider& collider,
	float linearDamping,
	float angularDamping,
	bool allowSleep,
	bool awake,
	bool fixedRotation,
	bool bullet,
	b2BodyType type,
	bool enabled,
	float gravityScale)
{
	glm::vec2 position{ transform->getPosition() };

	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(position.x, position.y);
	bodyDef.angle = glm::radians(transform->getRotation());
	bodyDef.type = type;
	bodyDef.linearDamping = linearDamping;
	bodyDef.angularDamping = angularDamping;
	bodyDef.allowSleep = allowSleep;
	bodyDef.awake = awake;
	bodyDef.fixedRotation = fixedRotation;
	bodyDef.bullet = bullet;
	bodyDef.enabled = enabled;
	bodyDef.gravityScale = gravityScale;

	body = PhysicsEngine::addBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = collider.getShape();
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	// Creating a fixture data
	fixtureData = new FixtureData;

	b2Fixture* fixture{ body->CreateFixture(&fixtureDef) };

	// Assigning the user data pointer of the fixture
	uintptr_t ptr = reinterpret_cast<uintptr_t>(fixtureData);
	fixture->GetUserDataRef().pointer = ptr;

	fixtureData->mFixture = fixture;
	fixtureData->rigidbody = this;
}

RigidbodyPointer Rigidbody::create(
	TransformPointer transform,
	b2BodyType type,
	float gravityScale,
	bool fixedRotation,
	bool allowSleep,
	float linearDamping,
	float angularDamping,
	bool bullet,
	bool awake,
	bool enabled)
{

	Rigidbody* rigidbody = new Rigidbody(transform, linearDamping, angularDamping, allowSleep, awake, fixedRotation, bullet, type, enabled, gravityScale);
	std::shared_ptr<Rigidbody> pointer{ rigidbody };
	transform->addComponent(pointer);
	return rigidbody;
}

RigidbodyPointer Rigidbody::create(
	TransformPointer transform,
	Collider& collider,
	b2BodyType type,
	float gravityScale,
	bool fixedRotation,
	bool allowSleep,
	float linearDamping,
	float angularDamping,
	bool bullet,
	bool awake,
	bool enabled)
{

	Rigidbody* rigidbody = new Rigidbody(transform, collider, linearDamping, angularDamping, allowSleep, awake, fixedRotation, bullet, type, enabled, gravityScale);
	std::shared_ptr<Rigidbody> pointer{ rigidbody };
	transform->addComponent(pointer);
	return rigidbody;
}

Rigidbody::~Rigidbody()
{
	// Removing the body from box2d
	PhysicsEngine::destroyBody(this->body);
	// Deleting the fixture data created on the heap
	delete fixtureData;
	// Indicating the rigidbody got destroyed
	Logger::destructorMessage("Rigidbody");
}

void Rigidbody::update()
{
}

void Rigidbody::updateTransform()
{
	b2Vec2 position{ body->GetPosition() };
	this->transform->setPosition(glm::vec2(position.x, position.y));

	this->transform->setRotation(glm::degrees(body->GetAngle()));
}

void Rigidbody::setPosition(glm::vec2 position, bool alsoSetTransformPosition)
{
	body->SetTransform(b2Vec2(position.x, position.y), body->GetAngle());
}
