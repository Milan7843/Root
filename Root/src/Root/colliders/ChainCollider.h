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
	 */
	static std::shared_ptr<Collider> create(std::vector<glm::vec2>& points,
		LayerMask selfLayerMask = LAYER_0,
		LayerMask interactionLayerMask = LAYER_ALL);

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
	 */
	ChainCollider(std::vector<glm::vec2>& points,
		LayerMask selfLayerMask,
		LayerMask interactionLayerMask);

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

