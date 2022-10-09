#pragma once

#include "Root/Root.h"

#include "Root/components/Component.h" // Base class
#include "Root/Gradient.h"
#include <stbi_utility/TextureData.h>

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>

#define ParticleSystemPointer ParticleSystem*

class Transform;

struct ParticleDrawData
{
	glm::vec2 position;
	glm::vec3 color;
	glm::vec2 size;
	float rotation;
};

struct ParticleUpdateData
{
	glm::vec2 velocity;
	float aliveTime;
	float lifeTime;
	float rotationVelocity;
};

enum SimulationSpace
{
	WORLD_SPACE,
	LOCAL_SPACE
};

enum EmissionMode
{
	ARC_EMISSION,
	ORTHOGONAL_EMISSION
};

enum OnFinish
{
	STOP,
	REPEAT,
	DESTROY_SELF,
	DESTROY_TRANSFORM
};

class ParticleSystem : public Component
{
public:

	static ParticleSystemPointer create(TransformPointer transform);

	~ParticleSystem();

	void start() override;

	void update() override;

	void render(float renderDepth) override;

	/**
	 * Start a new emission period which will last 'emissionDuration' seconds.
	 * Calling this function during active emission will reset the timer.
	 */
	void play();

	/**
	 * Stop the current emission.
	 */
	void stop();

	/**
	 * Set the particle size which stays constant over its lifetime.
	 * 
	 * \param size: the particle size.
	 */
	void setConstantSizeOverLifeTime(glm::vec2 size);

	/**
	 * Set the particle size which changes over its lifetime.
	 * The lifetime is normalized to [0, 1].
	 *
	 * \param sizeOverLifeTimeGradient: the particle size gradient.
	 */
	void setSizeOverLifeTimeGradient(Gradient<glm::vec2> sizeOverLifeTimeGradient);

	/**
	 * Set the particle drag which stays constant over its lifetime.
	 *
	 * \param drag: the drag gradient.
	 */
	void setConstantDragOverLifeTime(float drag);

	/**
	 * Set the particle drag which changes over its lifetime.
	 * The lifetime is normalized to [0, 1].
	 *
	 * \param dragOverLifeTimeGradient: the particle drag gradient.
	 */
	void setDragOverLifeTimeGradient(Gradient<float> dragOverLifeTimeGradient);

	/**
	 * Set the particle color which stays constant over its lifetime.
	 *
	 * \param color: the particle color.
	 */
	void setConstantColorOverLifeTime(glm::vec3 color);

	/**
	 * Set the particle color which changes over its lifetime.
	 * The lifetime is normalized to [0, 1].
	 *
	 * \param color: the particle color gradient.
	 */
	void setColorOverLifeTimeGradient(Gradient<glm::vec3> colorOverLifeTimeGradient);

	/**
	 * Set what this particle system should do when done.
	 * Options :
		STOP,
		REPEAT,
		DESTROY_SELF,
		DESTROY_TRANSFORM
	 * 
	 * \param onFinish: what this particle system should do when done.
	 */
	void setOnFinish(OnFinish onFinish);

	/**
	 * Set the particle limit.
	 *
	 * \param particleLimit: the new particle limit.
	 */
	void setParticleLimit(unsigned int particleLimit);

	/**
	 * Set the sprite for this particle system.
	 * 
	 * \param spritePath: the path to the sprite.
	 */
	void setSprite(const char* spritePath);

	/**
	 * Set the emission mode.
	 * 
	 * \param mode: the new emission mode.
	 */
	void setEmissionMode(EmissionMode mode);

	/**
	 * Set the emission duration per loop.
	 * 
	 * \param emissionTime: the new emission duration.
	 */
	void setEmissionDuration(float emissionDuration);

	/**
	 * Set the emission rate (number of particles emitted per second).
	 * 
	 * \param emissionRate; the new emission rate.
	 */
	void setEmissionRate(float emissionRate);

	/**
	 * Set the emission radius (distance from position a particle can spawn).
	 *
	 * \param emissionRadius: the new emission radius.
	 */
	void setEmissionRadius(float emissionRadius);

	/**
	 * Set the minimum angle for arc emission mode.
	 * 
	 * \param arcModeMinAngle: the minimum angle.
	 */
	void setArcEmissionModeMinAngle(float arcModeMinAngle);

	/**
	 * Set the maximum angle for arc emission mode.
	 *
	 * \param arcModeMaxAngle: the maximum angle.
	 */
	void setArcEmissionModeMaxAngle(float arcModeMaxAngle);

	/**
	 * Set the angle for orthogonal emission mode.
	 *
	 * \param orthogonalModeAngle: the new angle.
	 */
	void setOrthogonalModeAngle(float orthogonalModeAngle);

	/**
	 * Set the gravity (acceleration of the particles per second).
	 *
	 * \param gravity: the new gravity.
	 */
	void setGravity(glm::vec2 gravity);

	/**
	 * Set the wind (acceleration of the particles per second).
	 *
	 * \param gravity: the new wind.
	 */
	void setWind(glm::vec2 wind);

	/**
	 * Set the minimum particle life time.
	 * 
	 * \param minLifeTime: the new minimum life time.
	 */
	void setMinLifeTime(float minLifeTime);

	/**
	 * Set the maximum particle life time.
	 *
	 * \param maxLifeTime: the new maximum life time.
	 */
	void setMaxLifeTime(float maxLifeTime);

	/**
	 * Set the particle life time.
	 *
	 * \param lifeTime: the new life time for all particles.
	 */
	void setLifeTime(float lifeTime);

	/**
	 * Set the minimum particle emission velocity.
	 *
	 * \param minEmissionVelocity: the new minimum emission velocity.
	 */
	void setMinEmissionVelocity(float minEmissionVelocity);

	/**
	 * Set the maximum particle emission velocity.
	 *
	 * \param maxEmissionVelocity: the new maximum emission velocity.
	 */
	void setMaxEmissionVelocity(float maxEmissionVelocity);

	/**
	 * Set the particle emission velocity.
	 *
	 * \param emissionVelocity: the new emission velocity for all particles.
	 */
	void setEmissionVelocity(float emissionVelocity);

	/**
	 * Set the minimum particle rotation emission velocity.
	 *
	 * \param minRotationEmissionVelocity: the new minimum rotation emission velocity.
	 */
	void setMinRotationEmissionVelocity(float minRotationEmissionVelocity);

	/**
	 * Set the maximum particle rotation emission velocity.
	 *
	 * \param maxRotationEmissionVelocity: the new maximum rotation emission velocity.
	 */
	void setMaxRotationEmissionVelocity(float maxRotationEmissionVelocity);

	/**
	 * Set the particle rotation emission velocity.
	 *
	 * \param maxRotationEmissionVelocity: the new rotation emission velocity for all particles.
	 */
	void setRotationEmissionVelocity(float rotationEmissionVelocity);

	/**
	 * Set the minimum particle emission rotation at emission (degrees).
	 *
	 * \param minEmissionRotation: the new minimum rotation.
	 */
	void setMinEmissionRotation(float minEmissionRotation);

	/**
	 * Set the maximum particle emission rotation at emission (degrees).
	 *
	 * \param maxEmissionRotation: the new maximum rotation.
	 */
	void setMaxEmissionRotation(float maxEmissionRotation);

	/**
	 * Set the particle emission rotation (degrees).
	 *
	 * \param emissionRotation: the new rotation for all particles at emission.
	 */
	void setEmissionRotation(float emissionRotation);

	/**
	 * Set the rotation to follow the velocity.
	 * Overwrites all other rotation input.
	 * 
	 * \param letRotationFollowVelocity: whether to make the rotation folow the velocity.
	 */
	void setRotationFollowVelocity(bool letRotationFollowVelocity);

private:

	ParticleSystem();

	void writeDataToVAO();

	void emitParticle();

	glm::vec2 getRandomDirection();

	glm::vec2 getRandomPosition(glm::vec2 velocity);

	unsigned int VAO{ 0 };
	unsigned int VBO{ 0 };

	std::vector<ParticleDrawData> particleDrawData;
	std::vector<ParticleUpdateData> particleUpdateData;

	bool emitting{ false };
	SimulationSpace simulationSpace { WORLD_SPACE };
	float emissionDuration{ 3.0f };
	float emissionRate{ 6.0f }; // In emissions per second
	float currentEmissionTime{ 0.0f };
	float particlesEmittedThisRun{ 0 };

	bool useTexture{ false };
	unsigned int textureID{ 0 };

	EmissionMode emissionMode{ ARC_EMISSION };
	float emissionRadius{ 0.2f };
	float arcModeMinAngle{ 0.0f };
	float arcModeMaxAngle{ 360.0f };
	float orthogonalModeAngle{ 0.0f };

	glm::vec2 gravity{ 0.0f, -1.0f };
	float minLifeTime{ 3.0f };
	float maxLifeTime{ 3.0f };
	float minEmissionVelocity{ 1.0f };
	float maxEmissionVelocity{ 1.2f };
	float minRotationEmissionVelocity{ -360.0f };
	float maxRotationEmissionVelocity{ 360.0f };
	float minEmissionRotation{ 0.0f };
	float maxEmissionRotation{ 360.0f };
	bool letRotationFollowVelocity{ false };
	glm::vec2 wind{ 0.0f, 0.0f };

	unsigned int particleLimit{ 25 };

	OnFinish onFinish{ STOP };

	Gradient<glm::vec2> sizeOverLifeTimeGradient{ Gradient(glm::vec2(1.0f)) };
	Gradient<float> dragOverLifeTimeGradient{ Gradient(0.03f) };
	Gradient<glm::vec3> colorOverLifeTimeGradient{ Gradient(glm::vec3(1.0f)) };
};

