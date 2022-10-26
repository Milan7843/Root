#include "ChainCollider.h"

ChainCollider::ChainCollider(std::vector<glm::vec2>& points)
{
	setPoints(points);
}

ChainCollider::~ChainCollider()
{
	Logger::destructorMessage("Polygon collider");
}

void ChainCollider::setPoints(std::vector<glm::vec2>& points)
{
	unsigned int pointCount = points.size();

	if (pointCount < 3)
	{
		Logger::logError("Cannot create a chain collider from less than 2 points. Found: " + std::to_string(pointCount) + ".");
		return;
	}

	this->points = points;
}

b2Shape* ChainCollider::getShape()
{
	// If the shape hasn't already been set, make it first
	if (shape == nullptr)
	{
		unsigned int pointCount = points.size();

		std::vector<b2Vec2> b2Points(pointCount);

		// Copying the data to the b2Vec2 format
		for (unsigned int i{ 1 }; i < pointCount - 1; i++)
		{
			b2Points[i] = b2Vec2(points[i].x, points[i].y);
		}

		// Creating the chain shape and setting the vertices
		shape = new b2ChainShape;
		// Accounting for 2 ghost points: one at index 0 and one at the end.
		shape->CreateChain(&b2Points[1], pointCount - 2, b2Points[0], b2Points[b2Points.size()-1]);
	}
	return shape;
}
