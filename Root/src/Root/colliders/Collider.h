#pragma once

#include <Root/Logger.h>

#include <box2d/b2_body.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define ColliderPointer std::shared_ptr<Collider>

class Collider
{
public:

	~Collider()
	{
		glDeleteVertexArrays(1, &debugVAO);
	}

	virtual b2Shape* getShape() { return nullptr; }

	virtual void renderDebugView() { }

protected:

	Collider() {}

	unsigned int debugVAO{ 0 };
};

