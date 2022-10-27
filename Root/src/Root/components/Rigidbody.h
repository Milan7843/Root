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

	/**
	 * Set the linear velocity of the center of mass.
	 * 
	 * \param velocity: the new linear velocity of the center of mass.
	 */
	void setLinearVelocity(const glm::vec2& velocity);

	/**
	 * Get the linear velocity of the center of mass.
	 *
	 * \returns the linear velocity of the center of mass.
	 */
	glm::vec2 getLinearVelocity() const;

	/**
	 * Set the angular velocity.
	 *
	 * \param velocity: the new angular velocity in degrees/second.
	 */
	void setAngularVelocity(float velocity);

	/**
	 * Get the angular velocity.
	 *
	 * \returns the angular velocity in degrees/second.
	 */
	/// Get the angular velocity.
	/// @return the angular velocity in radians/second.
	float getAngularVelocity() const;


	/**
	 * Apply a force at a world point. If the force is not
	 * applied at the center of mass, it will generate a torque and
	 * affect the angular velocity. This wakes up the body.
	 * 
	 * \param force: the world force vector, usually in Newtons (N).
	 * \param point: the world position of the point of application.
	 * \param wake: whether to also wake up the body.
	 */
	void applyForce(const glm::vec2& force, const glm::vec2& point, bool wake);

	/**
	 * Apply a force to the center of mass. This wakes up the body.
	 *
	 * \param force: the world force vector, usually in Newtons (N).
	 * \param wake: whether to also wake up the body.
	 */
	void applyForceToCenter(const glm::vec2& force, bool wake);

	/**
	 * Apply a torque. This affects the angular velocity
	 * without affecting the linear velocity of the center of mass.
	 * 
	 * \param torque: about the z-axis (out of the screen), usually in N-m.
	 * \param wake: whether to also wake up the body.
	 */
	void applyTorque(float torque, bool wake);

	/**
	 * Apply an impulse at a point. This immediately modifies the velocity.
	 * It also modifies the angular velocity if the point of application
	 * is not at the center of mass. This wakes up the body.
	 * 
	 * \param impulse: the world impulse vector, usually in N-seconds or kg-m/s.
	 * \param point: the world position of the point of application.
	 * \param wake: whether to also wake up the body.
	 */
	void applyLinearImpulse(const glm::vec2& impulse, const glm::vec2& point, bool wake);

	/**
	 * Apply an impulse to the center of mass. This immediately modifies the velocity.
	 *
	 * \param impulse: the world impulse vector, usually in N-seconds or kg-m/s.
	 * \param wake: whether to also wake up the body.
	 */
	void applyLinearImpulseToCenter(const glm::vec2& impulse, bool wake);

	/**
	 * Apply an angular impulse.
	 *
	 * \param impulse: the angular impulse in units of kg*m*m/s.
	 * \param wake: whether to also wake up the body.
	 */
	void applyAngularImpulse(float impulse, bool wake);

	/**
	 * Get the total mass of the body.
	 *
	 * \returns the mass, usually in kilograms (kg).
	 */
	float getMass() const;

	/**
	 * Get the rotational inertia of the body about the local origin.
	 *
	 * \returns the rotational inertia, usually in kg-m^2.
	 */
	float getInertia() const;

	/*
	/// Get the mass data of the body.
	/// @return a struct containing the mass, inertia and center of the body.
	b2MassData GetMassData() const;

	/// Set the mass properties to override the mass properties of the fixtures.
	/// Note that this changes the center of mass position.
	/// Note that creating or destroying fixtures can also alter the mass.
	/// This function has no effect if the body isn't dynamic.
	/// @param data the mass properties.
	void SetMassData(const b2MassData* data);
	
	/// This resets the mass properties to the sum of the mass properties of the fixtures.
	/// This normally does not need to be called unless you called SetMassData to override
	/// the mass and you later want to reset the mass.
	void ResetMassData();
	*/

	/**
	 * Get the world linear velocity of a world point attached to this body.
	 *
	 * \param worldPoint: a point in world coordinates.
	 * \returns the world velocity of a point.
	 */
	glm::vec2 getLinearVelocityFromWorldPoint(const glm::vec2& worldPoint) const;

	/**
	 * Get the world velocity of a local point.
	 *
	 * \param localPoint: a point in local coordinates.
	 * \returns the world velocity of a point.
	 */
	glm::vec2 getLinearVelocityFromLocalPoint(const glm::vec2& localPoint) const;

	/// Get the linear damping of the body.
	float getLinearDamping() const;

	/// Set the linear damping of the body.
	void setLinearDamping(float linearDamping);

	/// Get the angular damping of the body.
	float getAngularDamping() const;

	/// Set the angular damping of the body.
	void setAngularDamping(float angularDamping);

	/// Get the gravity scale of the body.
	float getGravityScale() const;

	/// Set the gravity scale of the body.
	void setGravityScale(float scale);

	/// Set the type of this body. This may alter the mass and velocity.
	void setType(b2BodyType type);

	/// Get the type of this body.
	b2BodyType getType() const;

	/// Should this body be treated like a bullet for continuous collision detection?
	void setBullet(bool flag);

	/// Is this body treated like a bullet for continuous collision detection?
	bool isBullet() const;

	/// You can disable sleeping on this body. If you disable sleeping, the
	/// body will be woken.
	void setSleepingAllowed(bool flag);

	/// Is this body allowed to sleep
	bool isSleepingAllowed() const;

	/**
	 * Set the sleep state of the body. A sleeping body has very
	 * low CPU cost.
	 * 
	 * \param flag: set to true to wake the body, false to put it to sleep.
	 */
	void setAwake(bool flag);

	/**
	 * Get the sleeping state of this body.
	 *
	 * \returns true if the body is awake.
	 */
	bool isAwake() const;

	/**
	 * Allow a body to be disabled. A disabled body is not simulated and cannot
	 * be collided with or woken up.
	 * If you pass a flag of true, all fixtures will be added to the broad-phase.
	 * If you pass a flag of false, all fixtures will be removed from the
	 * broad-phase and all contacts will be destroyed.
	 * Fixtures and joints are otherwise unaffected. You may continue
	 * to create/destroy fixtures and joints on disabled bodies.
	 * Fixtures on a disabled body are implicitly disabled and will
	 * not participate in collisions, ray-casts, or queries.
	 * Joints connected to a disabled body are implicitly disabled.
	 * An diabled body is still owned by a b2World object and remains
	 * in the body list.
	 */
	void setEnabled(bool flag);

	/**
	 * Get whether this object is enabled.
	 * 
	 * \returns whether this object is enabled.
	 */
	bool isEnabled() const;

	/**
	 * Set this body to have fixed rotation. This causes the mass
	 * to be reset.
	 */
	void setFixedRotation(bool flag);

	/// Does this body have fixed rotation?
	bool isFixedRotation() const;

private:

	Rigidbody(TransformPointer transform, float linearDamping, float angularDamping, bool allowSleep, bool awake, bool fixedRotation, bool bullet, b2BodyType type, bool enabled, float gravityScale);
	Rigidbody(TransformPointer transform, Collider& collider, float linearDamping, float angularDamping, bool allowSleep, bool awake, bool fixedRotation, bool bullet, b2BodyType type, bool enabled, float gravityScale);

	FixtureData* fixtureData;
	b2Fixture* fixture;
	b2Body* body;
};
