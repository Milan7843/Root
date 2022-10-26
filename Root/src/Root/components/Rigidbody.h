#pragma once

#include "Root/components/Component.h" // Base class
#include "Root/colliders/Collider.h"

#include <box2d/b2_body.h>
#include <glm/glm.hpp>

#include <memory>

#define RigidbodyPointer Rigidbody*

class Transform;

// TODO change this to somehow only be in Transform.h
#define TransformPointer Transform*

// User data
struct FixtureData
{
	Rigidbody* rigidbody;
	b2Fixture* mFixture{};
};

#define STATIC		b2_staticBody
#define KINEMATIC	b2_kinematicBody
#define DYNAMIC		b2_dynamicBody


class Rigidbody : public Component
{
public:

	~Rigidbody();

	/**
	 * Create a new rigidbody.
	 * Will automatically add this component to the given transform.
	 *
	 * \param transform:		the transform to add this component to.
	 * \param type:				the body type: static, kinematic, or dynamic (STATIC, KINEMATIC, DYNAMIC).
	 * \param gravityScale:		scale the gravity applied to this body.
	 * \param bullet:			Is this a fast moving body that should be prevented from tunneling through
	 * 							other moving bodies? Note that all bodies are prevented from tunneling through
	 * 							kinematic and static bodies. This setting is only considered on dynamic bodies.
	 *							Increases processing time.
	 * \param fixedRotation:	should this body be prevented from rotating? Useful for characters.
	 * \param allowSleep:		set this flag to false if this body should never fall asleep. Note that
	 * 							this increases CPU usage.
	 * \param linearDamping:	linear damping is use to reduce the linear velocity. The damping parameter
	 * 							can be larger than 1.0f but the damping effect becomes sensitive to the
	 * 							time step when the damping parameter is large.
	 * 							Units are 1/time
	 * \param angularDamping:	angular damping is use to reduce the angular velocity. The damping parameter
	 * 							can be larger than 1.0f but the damping effect becomes sensitive to the
	 * 							time step when the damping parameter is large.
	 * 							Units are 1/time
	 * \param awake:			is this body initially awake or sleeping?
	 * \param enabled:			does this body start out enabled?
	 */
	static RigidbodyPointer create(
		TransformPointer transform,
		b2BodyType type = b2_staticBody,
		float gravityScale = 1.0f,
		bool bullet = false,
		bool fixedRotation = false,
		bool allowSleep = true,
		float linearDamping = 0.0f,
		float angularDamping = 0.0f,
		bool awake = true,
		bool enabled = true);

	/**
	 * Create a new rigidbody with a collider attached to it.
	 * Will automatically add this component to the given transform.
	 *
	 * \param transform:		the transform to add this component to.
	 * \param collider:			the collider that this rigidbody will get.
	 * \param type:				the body type: static, kinematic, or dynamic (STATIC, KINEMATIC, DYNAMIC).
	 * \param gravityScale:		scale the gravity applied to this body.
	 * \param bullet:			Is this a fast moving body that should be prevented from tunneling through
	 * 							other moving bodies? Note that all bodies are prevented from tunneling through
	 * 							kinematic and static bodies. This setting is only considered on dynamic bodies.
	 *							Increases processing time.
	 * \param fixedRotation:	should this body be prevented from rotating? Useful for characters.
	 * \param allowSleep:		set this flag to false if this body should never fall asleep. Note that
	 * 							this increases CPU usage.
	 * \param linearDamping:	linear damping is use to reduce the linear velocity. The damping parameter
	 * 							can be larger than 1.0f but the damping effect becomes sensitive to the
	 * 							time step when the damping parameter is large.
	 * 							Units are 1/time
	 * \param angularDamping:	angular damping is use to reduce the angular velocity. The damping parameter
	 * 							can be larger than 1.0f but the damping effect becomes sensitive to the
	 * 							time step when the damping parameter is large.
	 * 							Units are 1/time
	 * \param awake:			is this body initially awake or sleeping?
	 * \param enabled:			does this body start out enabled?
	 */
	static RigidbodyPointer create(
		TransformPointer transform,
		Collider& collider,
		b2BodyType type = b2_staticBody,
		float gravityScale = 1.0f,
		bool bullet = false,
		bool fixedRotation = false,
		bool allowSleep = true,
		float linearDamping = 0.0f,
		float angularDamping = 0.0f,
		bool awake = true,
		bool enabled = true);

	void update() override;

	/**
	 * Update the transform to have the position and rotation of the rigidbody.
	 */
	void updateTransform();

	/**
	 * Set the position of this rigidbody.
	 * 
	 * \param position: the new position.
	 * \param alsoSetTransformPosition: whether to also set the transform's position. Do not change this.
	 */
	void setPosition(glm::vec2 position, bool alsoSetTransformPosition = true);


private:

	Rigidbody(TransformPointer transform, float linearDamping, float angularDamping, bool allowSleep, bool awake, bool fixedRotation, bool bullet, b2BodyType type, bool enabled, float gravityScale);
	Rigidbody(TransformPointer transform, Collider& collider, float linearDamping, float angularDamping, bool allowSleep, bool awake, bool fixedRotation, bool bullet, b2BodyType type, bool enabled, float gravityScale);

	FixtureData* fixtureData;

	b2Body* body;
};
