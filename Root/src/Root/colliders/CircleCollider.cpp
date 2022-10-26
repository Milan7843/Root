#include "CircleCollider.h"

CircleCollider::CircleCollider(float radius)
	: radius(radius)
{
}

CircleCollider::~CircleCollider()
{
	Logger::destructorMessage("Circle collider");
}

b2Shape* CircleCollider::getShape()
{
	// If the shape hasn't already been set, make it first
	if (shape == nullptr)
	{
		shape = new b2CircleShape;
		shape->m_radius = 0.5f;
	}

	return shape;
}
