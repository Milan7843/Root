#pragma once

#include "Collider.h"

#include <glm/glm.hpp>

#include <box2d/b2_chain_shape.h>

#include <vector>

class LoopCollider : public Collider
{
public:

	/**
	 * Create a new loop collider.
	 *
	 * \param points:				the points that will make up the loop collider.
	 *								For inside collision, use clockwise winding order,
	 *								and for outside collision, use counter-clockwise winding order.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 * \param invertCollision:		whether to invert the collision (inside-outside).
	 *								Default = false.
	 */
	static std::shared_ptr<Collider> create(std::vector<glm::vec2>& points,
		LayerMask selfLayerMask = LAYER_0,
		LayerMask interactionLayerMask = LAYER_ALL,
		bool invertCollision = false);

	~LoopCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

	/**
	 * Set the inversion of the collider.
	 *
	 * \param invertCollision: whether to invert the collision (inside-outside).
	 * When the loop collider is not inverted:
	 * For inside collision, use clockwise winding order,
	 * and for outside collision, use counter-clockwise winding order.
	 * 
	 * When the loop collider is inverted:
	 * For inside collision, use counter-clockwise winding order,
	 * and for outside collision, use clockwise winding order.
	 */
	void setInverted(bool invertCollision);

private:

	/**
	 * Create a new loop collider.
	 *
	 * \param points:				the points that will make up the loop collider.
	 *								For inside collision, use clockwise winding order,
	 *								and for outside collision, use counter-clockwise winding order.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 * \param invertCollision:		whether to invert the collision (inside-outside).
	 *								Default = false.
	 */
	LoopCollider(std::vector<glm::vec2>& points,
		LayerMask selfLayerMask,
		LayerMask interactionLayerMask,
		bool invertCollision);

	void generateDebugVAO();

	// The points that make up this collider
	std::vector<glm::vec2> points;

	b2ChainShape* shape{ nullptr };

	// Whether the collisions are inverted
	bool inverted;

	// Give class TileGridCollider access
	friend class TileGridCollider;
};

