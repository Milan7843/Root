#pragma once

#include "Collider.h"

#include <box2d/b2_polygon_shape.h>

#include <vector>

class BoxCollider : public Collider
{
public:

	/**
	 * Create a new box collider.
	 *
	 * \param width:				the width of the box collider.
	 * \param height:				the height of the box collider.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 */
	static std::shared_ptr<Collider> create(float width = 1.0f, float height = 1.0f,
		LayerMask selfLayerMask = LAYER_0,
		LayerMask interactionLayerMask = LAYER_ALL);

	~BoxCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

private:

	/**
	 * Create a new box collider.
	 *
	 * \param width:				the width of the box collider.
	 * \param height:				the height of the box collider.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 */
	BoxCollider(float width, float height,
		LayerMask selfLayerMask,
		LayerMask interactionLayerMask);

	void generateDebugVAO();

	float width;
	float height;

	b2PolygonShape* shape{ nullptr };
};
