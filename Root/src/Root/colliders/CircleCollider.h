#pragma once

#include "Collider.h"

#include <box2d/b2_circle_shape.h>

#include <glm/glm.hpp>

#include <vector>

class CircleCollider : public Collider
{
public:

	/**
	 * Create a new circle collider.
	 * 
	 * \param radius: the radius of the circle collider.
	 */
	static std::shared_ptr<Collider> create(float radius = 0.5f);

	~CircleCollider();

	b2Shape* getShape() override;

	void renderDebugView() override;

private:

	/**
	 * Create a new circle collider.
	 *
	 * \param radius: the radius of the circle collider.
	 */
	CircleCollider(float radius);

	void generateDebugVAO();

	float radius;

	unsigned int debugResolution{ 12 };

	b2CircleShape* shape{ nullptr };
};

