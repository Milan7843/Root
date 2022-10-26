#include "PolygonCollider.h"

PolygonCollider::PolygonCollider(std::vector<glm::vec2>& points)
{
	setPoints(points);
}

PolygonCollider::~PolygonCollider()
{
	Logger::destructorMessage("Polygon collider");
}

void PolygonCollider::setPoints(std::vector<glm::vec2>& points)
{
	unsigned int pointCount = points.size();

	if (pointCount < 3)
	{
		Logger::logError("Cannot create a polygon collider from less than 2 points. Found: " + std::to_string(pointCount) + ".");
		return;
	}

	this->points = points;
}

b2Shape* PolygonCollider::getShape()
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

		// Creating the polygon shape and setting the vertices
		shape = new b2PolygonShape;
		shape->Set(&b2Points[0], pointCount);
	}
	return shape;
}
