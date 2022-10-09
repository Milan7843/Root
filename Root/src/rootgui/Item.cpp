#include "Item.h"

RootGUIComponent::Item::Item(unsigned int windowWidth, unsigned int windowHeight,
	glm::vec2 position, glm::vec2 size, glm::vec2 scale)
	: position(position)
	, size(glm::vec2(size.x / windowWidth, size.y / windowHeight))
	, scale(scale)
	, previousWindowHeight(windowHeight)
	, previousWindowWidth(windowWidth)
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

glm::vec2 RootGUIComponent::Item::getPosition(unsigned int windowWidth, unsigned int windowHeight)
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

	glm::vec2 realSize{ getSize(windowWidth, windowHeight) };
	return glm::vec2(position.x - realSize.x * horizontalAnchor, position.y - realSize.y * verticalAnchor);
}

glm::vec2 RootGUIComponent::Item::getSize(unsigned int windowWidth, unsigned int windowHeight)
{
	return glm::vec2(size.x
		* ((float)previousWindowWidth / (float)windowWidth)
		/ ((float)previousWindowHeight / (float)windowHeight),
		size.y);
		//size.y * (float)previousWindowHeight / (float)windowHeight);
}
