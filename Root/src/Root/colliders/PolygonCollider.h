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
	 * \param invertCollision: whether to invert the collisions from outside to inside.
	 */
	static std::shared_ptr<Collider> create(std::vector<glm::vec2>& points,
		bool invertCollision = false);

	~PolygonCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

	/**
	 * Set the inversion of the collider.
	 *
	 * \param invertCollision: whether to invert the collision (inside-outside).
	 */
	void setInverted(bool invertCollision);

private:

	/**
	 * Create a new polygon collider.
	 *
	 * \param points: the points that will make up the polygon collider.
	 * Must be in counter-clockwise order.
	 * \param invertCollision: whether to invert the collisions from outside to inside.
	 */
	PolygonCollider(std::vector<glm::vec2>& points,
		bool invertCollision);

	/**
	 * Set the points of the polygon collider.
	 *
	 * \param points: the points that will make up the polygon collider.
	 */
	void setPoints(std::vector<glm::vec2>& points);

	void generateDebugVAO();

	// The points that make up this collider (CCW)
	std::vector<glm::vec2> points;

	// Whether the collisions are inverted
	bool inverted;

	b2PolygonShape* shape{ nullptr };
};

