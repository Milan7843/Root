#include "BoxCollider.h"

BoxCollider::BoxCollider(float width, float height)
	: width(width)
	, height(height)
{
}

BoxCollider::~BoxCollider()
{
	Logger::destructorMessage("Box collider");
}

b2Shape* BoxCollider::getShape()
{
	// If the shape hasn't already been set, make it first
	if (shape == nullptr)
	{
		// Creating the polygon shape and setting it to be a box
		shape = new b2PolygonShape;
		shape->SetAsBox(width / 2.0f, height / 2.0f);
	}

	return shape;
}
