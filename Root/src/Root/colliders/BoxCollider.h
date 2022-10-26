#pragma once

#include "Collider.h"

#include <box2d/b2_polygon_shape.h>

class BoxCollider : public Collider
{
public:
	/**
	 * Create a new box collider.
	 *
	 * \param width: the width of the box collider;
	 * \param height: the height of the box collider;
	 */
	BoxCollider(float width = 1.0f, float height = 1.0f);

	~BoxCollider();

	b2Shape* getShape() override;

private:

	float width;
	float height;

	b2PolygonShape* shape{ nullptr };
};
