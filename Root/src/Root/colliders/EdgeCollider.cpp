#include "EdgeCollider.h"

std::shared_ptr<Collider> EdgeCollider::create(glm::vec2& point1, glm::vec2& point2,
    LayerMask selfLayerMask,
    LayerMask interactionLayerMask,
    bool sensor,
    float density,
    float friction)
{
	EdgeCollider* collider = new EdgeCollider(
        point1, point2,
        selfLayerMask, interactionLayerMask,
        sensor,
        density,
        friction);

	std::shared_ptr<EdgeCollider> pointer{ collider };
	return pointer;
}

EdgeCollider::EdgeCollider(glm::vec2& point1, glm::vec2& point2,
    LayerMask selfLayerMask,
    LayerMask interactionLayerMask,
    bool sensor,
    float density,
    float friction)
    : Collider(selfLayerMask, interactionLayerMask, density, friction, sensor)
	, point1(point1)
	, point2(point2)
{
}

EdgeCollider::~EdgeCollider()
{
	Logger::destructorMessage("Edge collider");
}

const std::vector<b2Shape*> EdgeCollider::getShapes()
{
    // If the shape hasn't already been set, make it first
    if (shape == nullptr)
    {
        b2Vec2 p1{ point1.x, point1.y };
        b2Vec2 p2{ point2.x, point2.y };

        // Creating the edge shape and setting the vertices
        shape = new b2EdgeShape;
        shape->SetTwoSided(p1, p2);
    }

    return std::vector<b2Shape*> { shape };
}

void EdgeCollider::renderDebugView()
{
    if (debugVAO == 0)
    {
        generateDebugVAO();
    }
    glBindVertexArray(debugVAO);

    glDrawArrays(GL_LINE_LOOP, 0, 2);

    glBindVertexArray(0);
}

void EdgeCollider::generateDebugVAO()
{
    // Creating the VAO for a square to draw 
    unsigned int VBO;

    glm::vec2 vertices[] = {
        point1, point2
    };

    // Generating the required objects
    glGenVertexArrays(1, &debugVAO);
    glGenBuffers(1, &VBO);

    // Making sure everything gets put on this specific VAO
    glBindVertexArray(debugVAO);

    // Binding the buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Putting the vertices into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Letting OpenGL know how to interpret the data:
    // 2 floats for position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbinding
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
