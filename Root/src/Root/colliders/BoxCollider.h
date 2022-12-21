#pragma once

#include "Collider.h"

#include <box2d/b2_polygon_shape.h>

#include <glm/gtx/quaternion.hpp>

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
	 * \param offset:				the offset of the collider from the center of the object is it attached to.
	 *								Default = (0, 0).
	 * \param rotation:				the rotation of the collider in degrees. Default = 0.
	 */
	static std::shared_ptr<Collider> create(float width = 1.0f, float height = 1.0f,
		LayerMask selfLayerMask = LAYER_0,
		LayerMask interactionLayerMask = LAYER_ALL,
		glm::vec2 offset = glm::vec2(0.0f),
		float rotation = 0.0f);

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
	 * \param offset:				the offset of the collider from the center of the object is it attached to.
	 *								Default = (0, 0).
	 * \param rotation:				the rotation of the collider in degrees. Default = 0.
	 */
	BoxCollider(float width, float height,
		LayerMask selfLayerMask,
		LayerMask interactionLayerMask,
		glm::vec2 offset,
		float rotation);

	void generateDebugVAO();

	float width;
	float height;

	b2PolygonShape* shape{ nullptr };

	glm::vec2 offset;
	float rotation;
};
