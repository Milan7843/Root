#pragma once

#include "Collider.h"

#include <glm/glm.hpp>

#include <box2d/b2_edge_shape.h>

#include <vector>

class EdgeCollider : public Collider
{
public:

	/**
	 * Create a new edge collider.
	 *
	 * \param point1:				the first point of the edge collider.
	 * \param point2:				the second point of the edge collider.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 * \param sensor:				whether the collider is a sensor. A sensor does not collide
	 *								with other objects, but it reports overlaps anyways.
	 * \param density:				the density of the collider. Default = 1.0.
	 * \param friction:				the friction of the collider. Default = 0.3.
	 */
	static std::shared_ptr<Collider> create(glm::vec2& point1,
		glm::vec2& point2,
		LayerMask selfLayerMask = LAYER_0,
		LayerMask interactionLayerMask = LAYER_ALL,
		bool sensor = false,
		float density = 1.0f,
		float friction = 0.3f);

	~EdgeCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

private:

	/**
	 * Create a new edge collider.
	 *
	 * \param point1:				the first point of the edge collider.
	 * \param point2:				the second point of the edge collider.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 * \param sensor:				whether the collider is a sensor. A sensor does not collide
	 *								with other objects, but it reports overlaps anyways.
	 * \param density:				the density of the collider. Default = 1.0.
	 * \param friction:				the friction of the collider. Default = 0.3.
	 */
	EdgeCollider(glm::vec2& point1, glm::vec2& point2,
		LayerMask selfLayerMask,
		LayerMask interactionLayerMask,
		bool sensor,
		float density,
		float friction);

	void generateDebugVAO();

	// The points that make up this collider
	glm::vec2 point1;
	glm::vec2 point2;

	b2EdgeShape* shape{ nullptr };
};

