#pragma once

#include "Collider.h"

#include <glm/glm.hpp>

#include <box2d/b2_polygon_shape.h>

#include <vector>

class PolygonCollider : public Collider
{
public:

	/**
	 * Create a new polygon collider.
	 *
	 * \param points: the points that will make up the polygon collider.
	 * Must be in counter-clockwise order.
	 */
	static std::shared_ptr<Collider> create(std::vector<glm::vec2>& points);

	~PolygonCollider();

	b2Shape* getShape() override;

	void renderDebugView() override;

private:

	/**
	 * Create a new polygon collider.
	 *
	 * \param points: the points that will make up the polygon collider.
	 * Must be in counter-clockwise order.
	 */
	PolygonCollider(std::vector<glm::vec2>& points);

	/**
	 * Set the points of the polygon collider.
	 *
	 * \param points: the points that will make up the polygon collider.
	 * Must be in counter-clockwise order.
	 */
	void setPoints(std::vector<glm::vec2>& points);

	void generateDebugVAO();

	// The points that make up this collider (CCW)
	std::vector<glm::vec2> points;

	b2PolygonShape* shape{ nullptr };
};

