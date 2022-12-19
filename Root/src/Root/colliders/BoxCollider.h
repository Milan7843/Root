#pragma once

#include "Collider.h"

#include <box2d/b2_polygon_shape.h>

#include <vector>

class BoxCollider : public Collider
{
public:

	/**
	 * Create a new box collider.
	 *
	 * \param width: the width of the box collider.
	 * \param height: the height of the box collider.
	 */
	static std::shared_ptr<Collider> create(float width = 1.0f, float height = 1.0f);

	~BoxCollider();

	const virtual std::vector<b2Shape*> getShapes() override;

	void renderDebugView() override;

private:

	/**
	 * Create a new box collider.
	 *
	 * \param width: the width of the box collider.
	 * \param height: the height of the box collider.
	 */
	BoxCollider(float width, float height);

	void generateDebugVAO();

	float width;
	float height;

	b2PolygonShape* shape{ nullptr };
};
