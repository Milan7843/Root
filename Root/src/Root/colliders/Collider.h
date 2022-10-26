#pragma once

#include <Root/Logger.h>

#include <box2d/b2_body.h>

#define ColliderPointer Collider*

class Collider
{
public:

	~Collider() {}

	virtual b2Shape* getShape() { return nullptr; }

protected:

	Collider() {}

};

