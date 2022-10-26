#include "LoopCollider.h"

LoopCollider::LoopCollider(std::vector<glm::vec2>& points)
	: points(points)
{
}

LoopCollider::~LoopCollider()
{
	Logger::destructorMessage("Loop collider");
}

b2Shape* LoopCollider::getShape()
{
	// If the shape hasn't already been set, make it first
	if (shape == nullptr)
	{
		unsigned int pointCount = points.size();

		std::vector<b2Vec2> b2Points(pointCount);

		// Copying the data to the b2Vec2 format
		for (unsigned int i{ 0 }; i < pointCount; i++)
		{
			b2Points[i] = b2Vec2(points[i].x, points[i].y);
		}

		// Creating the loop shape and setting the vertices
		shape = new b2ChainShape;
		shape->CreateLoop(&b2Points[0], pointCount);
	}
	return shape;
}
