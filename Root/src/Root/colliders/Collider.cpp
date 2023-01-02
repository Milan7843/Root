#include "Collider.h"

float Collider::getDensity()
{
	return density;
}

float Collider::getFriction()
{
	return friction;
}

bool Collider::isSensor()
{
	return sensor;
}
