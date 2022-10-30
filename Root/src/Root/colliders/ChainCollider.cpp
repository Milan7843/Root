#include "ChainCollider.h"

std::shared_ptr<Collider> ChainCollider::create(std::vector<glm::vec2>& points)
{
	ChainCollider* collider = new ChainCollider(points);
	std::shared_ptr<ChainCollider> pointer{ collider };
	return pointer;
}

ChainCollider::ChainCollider(std::vector<glm::vec2>& points)
{
	setPoints(points);
}

ChainCollider::~ChainCollider()
{
	Logger::destructorMessage("Polygon collider");
}

void ChainCollider::renderDebugView()
{
	if (debugVAO == 0)
	{
		generateDebugVAO();
	}
	glBindVertexArray(debugVAO);

	glDrawArrays(GL_LINE, 0, points.size());

	glBindVertexArray(0);
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

void ChainCollider::generateDebugVAO()
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
