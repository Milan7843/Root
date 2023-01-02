#include "CircleCollider.h"

std::shared_ptr<Collider> CircleCollider::create(float radius,
	LayerMask selfLayerMask,
	LayerMask interactionLayerMask,
	bool sensor,
	float density,
	float friction,
	glm::vec2 offset)
{
	CircleCollider* collider = new CircleCollider(
		radius,
		selfLayerMask,
		interactionLayerMask,
		sensor,
		density,
		friction,
		offset);

	std::shared_ptr<CircleCollider> pointer{ collider };
	return pointer;
}

CircleCollider::CircleCollider(float radius,
	LayerMask selfLayerMask,
	LayerMask interactionLayerMask,
	bool sensor,
	float density,
	float friction,
	glm::vec2 offset)
	: Collider(selfLayerMask, interactionLayerMask, density, friction, sensor)
	, radius(radius)
	, offset(offset)
{
}

CircleCollider::~CircleCollider()
{
	Logger::destructorMessage("Circle collider");
}

const std::vector<b2Shape*> CircleCollider::getShapes()
{
	// If the shape hasn't already been set, make it first
	if (shape == nullptr)
	{
		shape = new b2CircleShape;
		shape->m_radius = 0.5f;
		shape->m_p = b2Vec2(offset.x, offset.y);
	}

	return std::vector<b2Shape*> { shape };
}

void CircleCollider::renderDebugView()
{
	if (debugVAO == 0)
	{
		generateDebugVAO();
	}
	glBindVertexArray(debugVAO);

	glDrawArrays(GL_LINE_LOOP, 0, debugResolution);

	glBindVertexArray(0);
}

void CircleCollider::generateDebugVAO()
{
    // Creating the VAO for a square to draw 
    unsigned int VBO;

	std::vector<glm::vec2> points;

	// Generating the points
	for (unsigned int i{ 0 }; i < debugResolution; i++)
	{
		float angle{ ((float)i / (float)debugResolution) * 360.0f };
		points.push_back(offset + glm::vec2(glm::cos(glm::radians(angle)), glm::sin(glm::radians(angle))) * radius);
	}

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
