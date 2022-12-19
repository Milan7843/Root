#pragma once

#include "Collider.h"

#include <glm/glm.hpp>

#include <box2d/b2_edge_shape.h>

#include <vector>

class EdgeCollider : public Collider
{
public:

	/**
	 * Create a new edge collider.
	 *
	 * \param point1: the first point of the edge collider.
	 * \param point2: the second point of the edge collider.
	 */
	static std::shared_ptr<Collider> create(glm::vec2& point1, glm::vec2& point2);

	~EdgeCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

private:

	/**
	 * Create a new edge collider.
	 *
	 * \param point1: the first point of the edge collider.
	 * \param point2: the second point of the edge collider.
	 */
	EdgeCollider(glm::vec2& point1, glm::vec2& point2);

	void generateDebugVAO();

	// The points that make up this collider
	glm::vec2 point1;
	glm::vec2 point2;

	b2EdgeShape* shape{ nullptr };
};

