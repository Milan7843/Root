#pragma once

#include "Collider.h"

#include <box2d/b2_circle_shape.h>

class CircleCollider : public Collider
{
public:

	/**
	 * Create a new circle collider.
	 * 
	 * \param radius: the radius of the circle collider.
	 */
	CircleCollider(float radius = 0.5f);
	~CircleCollider();

	b2Shape* getShape() override;

private:
	float radius;

	b2CircleShape* shape{ nullptr };
};

