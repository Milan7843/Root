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

bool Component::internal_started()
{
	return internal_called_start;
}

void Component::internal_start()
{
	internal_called_start = true;
}

