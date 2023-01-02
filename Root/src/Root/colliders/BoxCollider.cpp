#include "BoxCollider.h"

std::shared_ptr<Collider> BoxCollider::create(float width, float height,
    LayerMask selfLayerMask,
    LayerMask interactionLayerMask,
    bool sensor,
    float density,
    float friction,
    glm::vec2 offset,
    float rotation)
{
	BoxCollider* collider = new BoxCollider(
        width, height,
        selfLayerMask,
        interactionLayerMask,
        sensor,
        density,
        friction,
        offset,
        rotation);

	std::shared_ptr<BoxCollider> pointer{ collider };
	return pointer;
}

BoxCollider::BoxCollider(float width, float height,
    LayerMask selfLayerMask,
    LayerMask interactionLayerMask,
    bool sensor,
    float density,
    float friction,
    glm::vec2 offset,
    float rotation)
    : Collider(selfLayerMask, interactionLayerMask, density, friction, sensor)
	, width(width)
	, height(height)
    , offset(offset)
    , rotation(rotation)
{
}

void BoxCollider::renderDebugView()
{
    if (debugVAO == 0)
    {
        generateDebugVAO();
    }
    glBindVertexArray(debugVAO);

    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glBindVertexArray(0);
}

BoxCollider::~BoxCollider()
{
	Logger::destructorMessage("Box collider");
}

const std::vector<b2Shape*> BoxCollider::getShapes()
{
	// If the shape hasn't already been set, make it first
	if (shape == nullptr)
	{
		// Creating the polygon shape and setting it to be a box
		shape = new b2PolygonShape;
		shape->SetAsBox(width / 2.0f, height / 2.0f, b2Vec2(offset.x, offset.y), glm::radians(rotation));
	}

    return std::vector<b2Shape*> { shape };
}

void BoxCollider::generateDebugVAO()
{
    // Creating the VAO for a square to draw 
    unsigned int VBO;

    glm::vec2 vertices[] = {
        // Positions
        glm::vec2( width / 2.0f,  height / 2.0f), // top right
        glm::vec2( width / 2.0f, -height / 2.0f), // bottom right
        glm::vec2(-width / 2.0f, -height / 2.0f), // bottom left
        glm::vec2(-width / 2.0f,  height / 2.0f)  // top left 
    };

    // Rotating the points to fit the rotation
    if (rotation != 0)
    {
        glm::mat4 rotationMatrix = glm::mat4(1.0f);

        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));

        std::cout << "ROTATION " << rotation << std::endl;

        for (int i = 0; i < 4; i++)
        {
            glm::vec4 point = glm::vec4(vertices[i].x, vertices[i].y, 0.0f, 1.0f);

            point = rotationMatrix * point;

            std::cout << "Point " << vertices[i].x << ", " << vertices[i].y << std::endl;

            vertices[i] = glm::vec2(point.x, point.y);
        }
    }

    // Adding the offset after possible rotation
    for (int i = 0; i < 4; i++)
    {
        vertices[i] += offset;
    }

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
