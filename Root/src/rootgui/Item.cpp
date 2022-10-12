#include "Item.h"

#include "internal/RootGUIInternal.h"

RootGUIComponent::Item::Item(
	glm::vec2 position, glm::vec2 size, glm::vec2 scale)
	: position(position)
	, size(glm::vec2(size.x / RootGUIInternal::getWindowWidth(), size.y / RootGUIInternal::getWindowHeight()))
	, scale(scale)
	, previousWindowHeight(RootGUIInternal::getWindowHeight())
	, previousWindowWidth(RootGUIInternal::getWindowWidth())
{
}

RootGUIComponent::Item::~Item()
{
	std::cout << "Item destroyed." << std::endl;
}

void RootGUIComponent::Item::setVerticalAnchorPoint(VerticalAnchorPoint newVerticalAnchorPoint)
{
	this->verticalAnchorPoint = newVerticalAnchorPoint;
}

void RootGUIComponent::Item::setHorizontalAnchorPoint(HorizontalAnchorPoint newHorizontalAnchorPoint)
{
	this->horizontalAnchorPoint = newHorizontalAnchorPoint;
}

glm::vec2 RootGUIComponent::Item::getPosition()
{
	float verticalAnchor;
	float horizontalAnchor;

	switch (verticalAnchorPoint) {
	case VerticalAnchorPoint::Bottom:
		verticalAnchor = 0.0f;
		break;
	case VerticalAnchorPoint::Middle:
		verticalAnchor = 0.5f;
		break;
	case VerticalAnchorPoint::Top:
		verticalAnchor = 1.0f;
		break;
	}

	switch (horizontalAnchorPoint) {
	case HorizontalAnchorPoint::Left:
		horizontalAnchor = 0.0f;
		break;
	case HorizontalAnchorPoint::Middle:
		horizontalAnchor = 0.5f;
		break;
	case HorizontalAnchorPoint::Right:
		horizontalAnchor = 1.0f;
		break;
	}

	glm::vec2 realSize{ getSize() };
	return glm::vec2(position.x - realSize.x * horizontalAnchor, position.y - realSize.y * verticalAnchor);
}

glm::vec2 RootGUIComponent::Item::getSize()
{
	return glm::vec2(size.x
		* ((float)previousWindowWidth / (float)RootGUIInternal::getWindowWidth())
		/ ((float)previousWindowHeight / (float)RootGUIInternal::getWindowHeight()),
		size.y);
		//size.y * (float)previousWindowHeight / (float)windowHeight);
}
