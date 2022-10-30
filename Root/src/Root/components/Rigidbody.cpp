#include "Rigidbody.h"

#include "Root/engine/PhysicsEngine.h"

#include <box2d/b2_settings.h>

void Rigidbody::generateDebugVAO()
{
}

Rigidbody::Rigidbody(TransformPointer transform,
	float linearDamping,
	float angularDamping,
	bool allowSleep,
	bool awake,
	bool fixedRotation,
	bool bullet,
	b2BodyType type,
	bool enabled,
	float gravityScale)
	: collider(nullptr)
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

	/*
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
	fixtureData->rigidbody = this;*/
}

Rigidbody::Rigidbody(TransformPointer transform,
	LayerMask selfLayerMask,
	LayerMask interactionLayerMask,
	std::shared_ptr<Collider> collider,
	float linearDamping,
	float angularDamping,
	bool allowSleep,
	bool awake,
	bool fixedRotation,
	bool bullet,
	b2BodyType type,
	bool enabled,
	float gravityScale)
	: collider(collider)
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
	fixtureDef.shape = collider->getShape();
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;

	// Setting the layer masks
	fixtureDef.filter.categoryBits = selfLayerMask;
	fixtureDef.filter.maskBits = interactionLayerMask;

	// Creating a fixture data
	fixtureData = new FixtureData;

	fixture = body->CreateFixture(&fixtureDef);

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
	bool bullet,
	bool fixedRotation,
	bool allowSleep,
	float linearDamping,
	float angularDamping,
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
	std::shared_ptr<Collider> collider,
	b2BodyType type,
	LayerMask selfLayerMask,
	LayerMask interactionLayerMask,
	float gravityScale,
	bool bullet,
	bool fixedRotation,
	bool allowSleep,
	float linearDamping,
	float angularDamping,
	bool awake,
	bool enabled)
{
	Rigidbody* rigidbody = new Rigidbody(transform, selfLayerMask, interactionLayerMask, collider, linearDamping, angularDamping, allowSleep, awake, fixedRotation, bullet, type, enabled, gravityScale);
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

	uintptr_t ptr = reinterpret_cast<uintptr_t>(nullptr);
	fixture->GetUserDataRef().pointer = ptr;

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


void Rigidbody::setLinearVelocity(const glm::vec2& velocity)
{
	b2Vec2 v{ velocity.x, velocity.y };
	this->body->SetLinearVelocity(v);
}

glm::vec2 Rigidbody::getLinearVelocity() const
{
	b2Vec2 velocity{ this->body->GetLinearVelocity() };
	return glm::vec2{ velocity.x, velocity.y };
}

void Rigidbody::setAngularVelocity(float velocity)
{
	float omega{ glm::radians(velocity) };
	body->SetAngularVelocity(omega);
}

float Rigidbody::getAngularVelocity() const
{
	float velocity{ glm::degrees(body->GetAngularVelocity()) };
	return velocity;
}

void Rigidbody::applyForce(const glm::vec2& force, const glm::vec2& point, bool wake)
{
	b2Vec2 f{ force.x, force.y };
	b2Vec2 p{ point.x, point.y };
	body->ApplyForce(f, p, wake);
}

void Rigidbody::applyForceToCenter(const glm::vec2& force, bool wake)
{
	b2Vec2 f{ force.x, force.y };
	body->ApplyForceToCenter(f, wake);
}

void Rigidbody::applyTorque(float torque, bool wake)
{
	body->ApplyTorque(torque, wake);
}

void Rigidbody::applyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point, bool wake)
{
	b2Vec2 i{ impulse.x, impulse.y };
	b2Vec2 p{ point.x, point.y };
	body->ApplyLinearImpulse(i, p, wake);
}

void Rigidbody::applyLinearImpulseToCenter(const glm::vec2& impulse, bool wake)
{
	b2Vec2 i{ impulse.x, impulse.y };
	body->ApplyLinearImpulseToCenter(i, wake);
}

void Rigidbody::applyAngularImpulse(float impulse, bool wake)
{
	body->ApplyAngularImpulse(impulse, wake);
}

float Rigidbody::getMass() const
{
	return body->GetMass();
}

float Rigidbody::getInertia() const
{
	return body->GetInertia();
}

glm::vec2 Rigidbody::getLinearVelocityFromWorldPoint(const glm::vec2& worldPoint) const
{
	b2Vec2 l{ body->GetLinearVelocityFromWorldPoint(b2Vec2{ worldPoint.x, worldPoint.y })};

	glm::vec2 linearVelocity{ l.x, l.y };
	return linearVelocity;
}

glm::vec2 Rigidbody::getLinearVelocityFromLocalPoint(const glm::vec2& localPoint) const
{
	b2Vec2 l{ body->GetLinearVelocityFromLocalPoint(b2Vec2{ localPoint.x, localPoint.y }) };

	glm::vec2 linearVelocity{ l.x, l.y };
	return linearVelocity;
}

float Rigidbody::getLinearDamping() const
{
	return body->GetLinearDamping();
}

void Rigidbody::setLinearDamping(float linearDamping)
{
	body->SetLinearDamping(linearDamping);
}

float Rigidbody::getAngularDamping() const
{
	return body->GetAngularDamping();
}

void Rigidbody::setAngularDamping(float angularDamping)
{
	body->SetAngularDamping(angularDamping);
}

float Rigidbody::getGravityScale() const
{
	return body->GetGravityScale();
}

void Rigidbody::setGravityScale(float scale)
{
	body->SetGravityScale(scale);
}

void Rigidbody::setType(b2BodyType type)
{
	body->SetType(type);
}

b2BodyType Rigidbody::getType() const
{
	return body->GetType();
}

void Rigidbody::setBullet(bool flag)
{
	body->SetBullet(flag);
}

bool Rigidbody::isBullet() const
{
	return body->IsBullet();
}

void Rigidbody::setSleepingAllowed(bool flag)
{
	body->SetSleepingAllowed(flag);
}

bool Rigidbody::isSleepingAllowed() const
{
	return body->IsSleepingAllowed();
}

void Rigidbody::setAwake(bool flag)
{
	body->SetAwake(flag);
}

bool Rigidbody::isAwake() const
{
	return body->IsAwake();
}

void Rigidbody::setEnabled(bool flag)
{
	body->SetEnabled(flag);
}

bool Rigidbody::isEnabled() const
{
	return body->IsEnabled();
}

void Rigidbody::setFixedRotation(bool flag)
{
	body->SetFixedRotation(flag);
}

bool Rigidbody::isFixedRotation() const
{
	return body->IsFixedRotation();
}

void Rigidbody::setSelfLayerMask(LayerMask mask)
{
	b2Filter filter{ fixture->GetFilterData() };
	filter.categoryBits = mask;
	fixture->SetFilterData(filter);
}

void Rigidbody::setInteractionLayerMask(LayerMask mask)
{
	b2Filter filter{ fixture->GetFilterData() };
	filter.maskBits = mask;
	fixture->SetFilterData(filter);
}

LayerMask Rigidbody::getSelfLayerMask()
{
	return fixture->GetFilterData().categoryBits;
}

LayerMask Rigidbody::getInteractionLayerMask()
{
	return fixture->GetFilterData().maskBits;
}

void Rigidbody::renderDebugView()
{
	// Getting the shader
	Shader* debugRenderShader{ Root::getDebugRenderShader() };

	// Setting shader variables
	debugRenderShader->use();
	debugRenderShader->setMat4("model", transform->getModelMatrixWithoutScale());
	debugRenderShader->setMat4("view", Root::getActiveCamera()->getTransform()->getInverseTransformMatrix());

	debugRenderShader->setMat4("projection", Root::getActiveCamera()->getProjectionMatrix());
	debugRenderShader->setVector3("lineColor", glm::vec3(0.0f, 1.0f, 0.0f));

	collider->renderDebugView();
}
