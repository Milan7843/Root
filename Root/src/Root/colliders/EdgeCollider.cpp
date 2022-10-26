#include "EdgeCollider.h"

EdgeCollider::EdgeCollider(glm::vec2& point1, glm::vec2& point2)
	: point1(point1)
	, point2(point2)
{
}

EdgeCollider::~EdgeCollider()
{
	Logger::destructorMessage("Edge collider");
}

b2Shape* EdgeCollider::getShape()
{
	// If the shape hasn't already been set, make it first
	if (shape == nullptr)
	{
		b2Vec2 p1{ point1.x, point1.y };
		b2Vec2 p2{ point2.x, point2.y };

		// Creating the edge shape and setting the vertices
		shape = new b2EdgeShape;
		shape->SetTwoSided(p1, p2);
	}
	return shape;
}
