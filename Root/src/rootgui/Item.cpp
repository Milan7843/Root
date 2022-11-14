#include "Item.h"

#include "internal/RootGUIInternal.h"

RootGUIComponent::Item::Item(
	glm::vec2 position,
	float rotation)
	: TransformBase(position, rotation, glm::vec2(1.0f))
	, previousWindowHeight(RootGUIInternal::getWindowHeight())
	, previousWindowWidth(RootGUIInternal::getWindowWidth())
{
}

RootGUIComponent::Item::~Item()
{
	std::cout << "Item destroyed." << std::endl;
}

void RootGUIComponent::Item::setVerticalScreenAnchorPoint(VerticalAnchorPoint newVerticalScreenAnchorPoint)
{
	this->verticalAnchorPoint = newVerticalScreenAnchorPoint;
}

void RootGUIComponent::Item::setHorizontalScreenAnchorPoint(HorizontalAnchorPoint newHorizontalScreenAnchorPoint)
{
	this->horizontalAnchorPoint = newHorizontalScreenAnchorPoint;
}

float RootGUIComponent::Item::getVerticalScreenAnchor()
{
	switch (verticalAnchorPoint)
	{
		case VerticalAnchorPoint::Bottom:
			return -1.0f;

		case VerticalAnchorPoint::Middle:
			return 0.0f;

		case VerticalAnchorPoint::Top:
			return 1.0f;
	}
	return 0.0f;
}

float RootGUIComponent::Item::getHorizontalScreenAnchor()
{
	switch (horizontalAnchorPoint)
	{
		case HorizontalAnchorPoint::Left:
			return -1.0f;

		case HorizontalAnchorPoint::Middle:
			return 0.0f;

		case HorizontalAnchorPoint::Right:
			return 1.0f;
	}
	return 0.0f;
}

glm::vec2 RootGUIComponent::Item::getPosition()
{
	return position;
}