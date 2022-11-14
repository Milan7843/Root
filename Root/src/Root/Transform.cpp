#include "Transform.h"

#include "Root/engine/RootEngine.h"

Transform::Transform(glm::vec2 position, float rotation, glm::vec2 scale, float renderDepth)
	: TransformBase(position, rotation, scale)
	, renderDepth(renderDepth)
{
}

Transform::~Transform()
{
	Logger::destructorMessage("Transform");
}

TransformPointer Transform::create(glm::vec2 position, float rotation, glm::vec2 scale, float renderDepth)
{
	Transform* transform = new Transform(position, rotation, scale, renderDepth);
	std::shared_ptr<Transform> pointer{ transform };
	Root::addTransform(pointer);
	return transform;
}

void Transform::destroy()
{
	// First removing each child
	for (Transform* child : derivedChildren)
	{
		child->destroy();
	}
	RootEngine::removeTransform(this);
}

void Transform::render(float parentRenderDepth, float renderDepthOffset)
{
	// Either use own renderdepth
	float usedRenderDepth{ renderDepth };

	// Or parent's
	if (renderDepth == -1.0f)
		usedRenderDepth = parentRenderDepth + renderDepthOffset;


	// Calling render() on each component attached to this Transform
	for (std::shared_ptr<Component> component : getComponents())
	{
		renderDepthOffset -= 0.001f;
		component->render(usedRenderDepth + renderDepthOffset);
	}
	// Then rendering each child
	for (Transform* child : derivedChildren)
	{
		renderDepthOffset -= 0.001f;
		child->render(usedRenderDepth, renderDepthOffset);
	}
}

std::string Transform::toString()
{
	std::stringstream stream;
	// Writing this object to the stream
	stream << "[Transform] " << name << ", tagged " << tag
		<< "\n > position: (" << position.x << ", " << position.y << ")"
		<< "\n > rotation: " << rotation
		<< "\n > scale: (" << scale.x << ", " << scale.y << ")"
		<< "\n > components: ["
		<< "\n";

	for (std::shared_ptr<Component> component : components)
	{
		stream << " >" << component->toString() << "\n";
	}

	stream << "\n > children: [\n";

	for (Transform* child : derivedChildren)
	{
		stream << " >" << child->toString() << "\n";
	}

	stream << "]" << std::endl;

	return stream.str();
}


void Transform::setParent(Transform* parent, bool alsoAddChild)
{
	// Check if the transform already had a parent, 
	// and remove it as a child from that parent if it did
	if (this->parent != NULL)
		this->parent->removeChild(this);

	// Setting new parent
	this->parent = parent;
	// Setting new derived parent
	this->derivedParent = parent;

	// Possibly adding child to new parent
	if (alsoAddChild && parent != NULL)
		parent->addChild(this, false);
}

Transform* Transform::getParent()
{
	return derivedParent; 
}

void Transform::addChild(Transform* child, bool alsoSetParent)
{
	children.push_back(child);
	derivedChildren.push_back(child);

	if (alsoSetParent)
		child->setParent(this, false);
}

bool Transform::removeChild(Transform* childToRemove)
{
	TransformBase::removeChild(childToRemove);

	for (unsigned int i{ 0 }; i < children.size(); i++)
	{
		// Comparing each current child to find one that matches
		if (derivedChildren[i] == childToRemove)
		{
			// If it does, unset its parent reference,
			derivedChildren[i]->parent = NULL;

			// remove it,
			children.erase(children.begin() + i);
			derivedChildren.erase(derivedChildren.begin() + i);

			// and return true to indicate a child was removed
			return true;
		}
	}
	return false;
}

void Transform::setRenderDepth(float renderDepth)
{
	if (renderDepth == -1.0f) {
		this->renderDepth = renderDepth;
		return;
	}
	this->renderDepth = glm::clamp(renderDepth, 0.0f, 10000.0f);
}

float Transform::getRenderDepth()
{
	return renderDepth;
}

std::vector<Transform*>& Transform::getChildren()
{
	return derivedChildren;
}

std::vector<std::shared_ptr<Component>>& Transform::getComponents()
{
	return components;
}

void Transform::addComponent(std::shared_ptr<Component> component)
{
	// If it is a rigidbody, save its reference
	if (typeid(*component) == typeid(Rigidbody))
	{
		// Check if a rigidbody was already attached
		if (attachedRigidbody != nullptr)
		{
			Logger::logError("Transform already contained a rigidbody! Only add ONE rigidbody per transform.");
		}
		else
		{
			attachedRigidbody = (RigidbodyPointer)component.get();
		}
	}

	components.push_back(component);
	component->setTransform(this);
}

bool Transform::removeComponent(std::shared_ptr<Component> component)
{
	for (unsigned int i{ 0 }; i < components.size(); i++)
	{
		if (components[i] == component)
		{
			components.erase(components.begin() + i);
			return true;
		}
	}

	return false;
}

bool Transform::removeComponent(ComponentPointer component)
{
	for (unsigned int i{ 0 }; i < components.size(); i++)
	{
		if (components[i].get() == component)
		{
			components.erase(components.begin() + i);
			return true;
		}
	}

	return false;
}

RigidbodyPointer Transform::getAttachedRigidbody()
{
	return attachedRigidbody;
}

void Transform::setPosition(glm::vec2 position)
{
	// Converting world position back to local
	glm::vec2 newLocalPosition{ worldPointToParentLocalPoint(position) };

	// Update position and set updated flag if the position changed
	if (this->position != newLocalPosition)
	{
		transformUpdated = true;
		this->position = newLocalPosition;
	}

	// Setting the rigidbody position to the current world position
	if (attachedRigidbody != nullptr)
	{
		attachedRigidbody->setPosition(position, false);
	}
}

void Transform::setLocalPosition(glm::vec2 position)
{
	// Update position and set updated flag if the position changed
	if (this->position != position)
	{
		transformUpdated = true;
		this->position = position;
	}

	// Setting the rigidbody position to the current world position
	if (attachedRigidbody != nullptr)
	{
		attachedRigidbody->setPosition(getPosition(), false);
	}
}

void Transform::movePosition(glm::vec2 offset)
{
	// Getting world position
	glm::vec2 worldPosition{ getPosition() };

	// Adding offset
	worldPosition = worldPosition + offset;

	// Converting world position back to local
	glm::vec2 newPosition{ worldPointToParentLocalPoint(worldPosition) };

	// Update position and set updated flag if the position changed
	if (this->position != newPosition)
	{
		transformUpdated = true;
		this->position = newPosition;
	}

	// Setting the rigidbody position to the current world position
	if (attachedRigidbody != nullptr)
	{
		attachedRigidbody->setPosition(worldPosition, false);
	}
}

void Transform::moveLocalPosition(glm::vec2 offset)
{
	// Update position and set updated flag if the position changed
	if (offset != glm::vec2(0.0f))
	{
		this->position += offset;
		transformUpdated = true;
	}

	// Setting the rigidbody position to the current world position
	if (attachedRigidbody != nullptr)
	{
		attachedRigidbody->setPosition(getPosition(), false);
	}
}
