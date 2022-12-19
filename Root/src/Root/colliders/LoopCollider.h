#pragma once

#include "Collider.h"

#include <glm/glm.hpp>

#include <box2d/b2_chain_shape.h>

#include <vector>

class LoopCollider : public Collider
{
public:

	/**
	 * Create a new loop collider.
	 *
	 * \param points: the points that will make up the loop collider.
	 * For inside collision, use clockwise winding order,
	 * and for outside collision, use counter-clockwise winding order.
	 */
	static std::shared_ptr<Collider> create(std::vector<glm::vec2>& points);

	~LoopCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

private:

	/**
	 * Create a new loop collider.
	 *
	 * \param points: the points that will make up the loop collider.
	 * For inside collision, use clockwise winding order,
	 * and for outside collision, use counter-clockwise winding order.
	 */
	LoopCollider(std::vector<glm::vec2>& points);

	void generateDebugVAO();

	// The points that make up this collider
	std::vector<glm::vec2> points;

	b2ChainShape* shape{ nullptr };

	// Give class TileGridCollider access
	friend class TileGridCollider;
};

