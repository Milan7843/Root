#include "Component.h"

#include "Root/Transform.h"

void Component::setTransform(Transform* transform)
{
	this->transform = transform;
}

Transform* Component::getTransform()
{
	return transform;
}
