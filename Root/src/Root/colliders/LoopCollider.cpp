#include "LoopCollider.h"

std::shared_ptr<Collider> LoopCollider::create(std::vector<glm::vec2>& points,
	LayerMask selfLayerMask,
	LayerMask interactionLayerMask,
	bool invertCollision,
	bool sensor,
	float density,
	float friction)
{
	LoopCollider* collider = new LoopCollider(
		points,
		selfLayerMask,
		interactionLayerMask,
		invertCollision,
		sensor,
		density,
		friction);

	std::shared_ptr<LoopCollider> pointer{ collider };
	return pointer;
}

void LoopCollider::setInverted(bool invertCollision)
{
	inverted = invertCollision;
}

LoopCollider::LoopCollider(std::vector<glm::vec2>& points,
	LayerMask selfLayerMask,
	LayerMask interactionLayerMask,
	bool invertCollision,
	bool sensor,
	float density,
	float friction)
	: Collider(selfLayerMask, interactionLayerMask, density, friction, sensor)
	, points(points)
	, inverted(invertCollision)
{
}

LoopCollider::~LoopCollider()
{
	Logger::destructorMessage("Loop collider");
}

const std::vector<b2Shape*> LoopCollider::getShapes()
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

		// The list must be reversed to invert the collisions
		if (inverted)
		{
			std::reverse(b2Points.begin(), b2Points.end());
		}

		// Creating the loop shape and setting the vertices
		shape = new b2ChainShape;
		shape->CreateLoop(&b2Points[0], pointCount);
	}

	return std::vector<b2Shape*> { shape };
}

void LoopCollider::renderDebugView()
{
	if (debugVAO == 0)
	{
		generateDebugVAO();
	}
	glBindVertexArray(debugVAO);

	glDrawArrays(GL_LINE_LOOP, 0, points.size());

	glBindVertexArray(0);
}

void LoopCollider::generateDebugVAO()
{
	// Creating the VAO

	unsigned int VBO;

	// Generating the required objects
	glGenVertexArrays(1, &debugVAO);
	glGenBuffers(1, &VBO);

	// Making sure everything gets put on this specific VAO
	glBindVertexArray(debugVAO);

	// Binding the buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Putting the vertices into the buffer
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(glm::vec2), points.data(), GL_STATIC_DRAW);

	// Letting OpenGL know how to interpret the data:
	// 2 floats for position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
