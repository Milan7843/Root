#pragma once

#include "Collider.h"

#include <box2d/b2_circle_shape.h>

#include <glm/glm.hpp>

#include <vector>

class CircleCollider : public Collider
{
public:

	/**
	 * Create a new circle collider.
	 * 
	 * \param radius:				the radius of the circle collider.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 * \param offset:				the offset of the collider from the center of the object is it attached to.
	 *								Default = (0, 0).
	 */
	static std::shared_ptr<Collider> create(float radius = 0.5f,
		LayerMask selfLayerMask = LAYER_0,
		LayerMask interactionLayerMask = LAYER_ALL,
		glm::vec2 offset = glm::vec2(0.0f));

	~CircleCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

private:

	/**
	 * Create a new circle collider.
	 *
	 * \param radius:				the radius of the circle collider.
	 * \param selfLayerMask:		a layer mask which defines which layers this rigidbody is on. Default = LAYER_0.
	 * \param interactionLayerMask:	a layer mask of all layers which this rigidbody can interact with. Default = LAYER_ALL.
	 * \param offset:				the offset of the collider from the center of the object is it attached to.
	 *								Default = (0, 0).
	 */
	CircleCollider(float radius,
		LayerMask selfLayerMask,
		LayerMask interactionLayerMask,
		glm::vec2 offset);

	void generateDebugVAO();

	float radius;
	glm::vec2 offset;

	unsigned int debugResolution{ 12 };

	b2CircleShape* shape{ nullptr };
};

