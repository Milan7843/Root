#pragma once

#include "Collider.h"

#include <glm/glm.hpp>

#include <box2d/b2_chain_shape.h>

#include <vector>

class ChainCollider : public Collider
{
public:

	/**
	 * Create a new chain collider.
	 *
	 * \param points: the points that will make up the chain collider.
	 * Important: the first and last of the points are ghost points,
	 * only used to make the transition smoother if multiple chains are linked together.
	 * These will not behave as regular chain points.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 * \param sensor:				whether the collider is a sensor. A sensor does not collide
	 *								with other objects, but it reports overlaps anyways.
	 * \param density:				the density of the collider. Default = 1.0.
	 * \param friction:				the friction of the collider. Default = 0.3.
	 */
	static std::shared_ptr<Collider> create(std::vector<glm::vec2>& points,
		LayerMask selfLayerMask = LAYER_0,
		LayerMask interactionLayerMask = LAYER_ALL,
		bool sensor = false,
		float density = 1.0f,
		float friction = 0.3f);

	~ChainCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

private:

	/**
	 * Create a new chain collider.
	 *
	 * \param points: the points that will make up the chain collider.
	 * Important: the first and last of the points are ghost points,
	 * only used to make the transition smoother if multiple chains are linked together.
	 * These will not behave as regular chain points.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 * \param sensor:				whether the collider is a sensor. A sensor does not collide
	 *								with other objects, but it reports overlaps anyways.
	 * \param density:				the density of the collider. Default = 1.0.
	 * \param friction:				the friction of the collider. Default = 0.3.
	 */
	ChainCollider(std::vector<glm::vec2>& points,
		LayerMask selfLayerMask,
		LayerMask interactionLayerMask,
		bool sensor,
		float density,
		float friction);

	/**
	 * Set the points of the chain collider.
	 *
	 * \param points: the points that will make up the chain collider.
	 * Important: the first and last of the points are ghost points,
	 * only used to make the transition smoother if multiple chains are linked together.
	 * These will not behave as regular chain points.
	 */
	void setPoints(std::vector<glm::vec2>& points);

	void generateDebugVAO();

	// The points that make up this collider
	std::vector<glm::vec2> points;

	b2ChainShape* shape{ nullptr };
};

