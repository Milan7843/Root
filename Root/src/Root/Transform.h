#pragma once

#include "Root/Logger.h"
#include "Root/Root.h"
#include "Root/Components.h"

#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <vector>
#include <typeinfo>
#include <typeindex>

#define TransformPointer Transform*

/**
 * Class which holds values for a 2D transformation.
 */
class Transform
{
public:

	/**
	 * Destructor for the Transform class.
	 */
	~Transform();

	/**
	 * Create a new Transform. Will automatically add this transform to the current scene.
	 *
	 * \param position: the world position of this transform [optional: default = (0, 0)]
	 * \param rotation: the rotation of this transform [optional: default = 0]
	 * \param scale: the scale of this transform [optional: default = (1, 1)]
	 * \param renderDepth: the depth at which this transform will be rendered.
	 * If this is not set or -1, the render depth of the parent will be used.
	 * The render depth is a scale from 0 (closest) to 10000 (furthest).
	 */
	static TransformPointer create(
		glm::vec2 position = glm::vec2(0.0f), // Initialize position to (0, 0)
		float rotation = 0.0f, // Initialize rotation to no rotation
		glm::vec2 scale = glm::vec2(1.0f), // Initialize scale to no scaling
		float renderDepth = 0.0f); // Initialize render depth to 0

	/**
	 * Destroy this transform.
	 * Will invalidate any pointers to this transform.
	 * Will also destroy all children, but not the parent.
	 * Any attached components without a reference will also be destroyed.
	 */
	void destroy();

	/**
	 * Render this Transform.
	 * Should not be called by the user.
	 * 
	 * \param parentRenderDepth: render the depth of the parent of this transform.
	 * \param renderDepthOffset: the offset on top of the render depth.
	 */
	void render(float parentRenderDepth = 0.0f, float renderDepthOffset = 0.0f);

	/**
	 * Get a string representing this Transform.
	 *
	 * \returns this Transform in a human-readable format.
	 */
	std::string toString();

	/**
	 * Set the parent transform of this transform.
	 * Having a parent will make the child transform's data in local space.
	 * This will also add this transform as a child to the parent.
	 * Note that adding a child to a transform via Transform::addChild() 
	 * will also set the child's parent using setParent(), 
	 * and therefore does not have to be called manually.
	 *
	 * \param parent: a shared_ptr to the transform that will be the parent.
	 * If this is NULL, the transform will have no parent
	 */
	void setParent(Transform* parent, bool alsoAddChild = true);

	/**
	 * Set this transform's name.
	 * Any transform's name starts out as empty ("").
	 * 
	 * \param name: the new name.
	 */
	void setName(std::string name);

	/**
	 * Get this transform's name.
	 * Any transform's name starts out as empty ("").
	 *
	 * \returns name: the name.
	 */
	std::string getName();

	/**
	 * Get the parent of this transform.
	 * 
	 * \returns the parent of this transform, or NULL if it does not have one.
	 */
	Transform* getParent();

	/**
	 * Add a child to this transform.
	 * This will also set the child's parent pointer to this transform.
	 * Note that setting a transform's parent via Transform::setParent() 
	 * will also set the parent's child using addChild(), 
	 * and therefore does not have to be called manually.
	 *
	 * \param child: a TransformPointer to the child.
	 */
	void addChild(Transform* child, bool alsoSetParent = true);

	/**
	 * Remove a child from this transform with the given shared_ptr.
	 * This will also set the child's parent pointer to NULL, indicating no parent.
	 *
	 * \param childToRemove: a TransformPointer to the child that should be removed.
	 * \returns whether a child was removed succesfully.
	 */
	bool removeChild(Transform* childToRemove);

	/**
	 * Remove all children from this transform with the given shared_ptr.
	 */
	void removeAllChildren();

	/**
	 * Set this transform's render depth.
	 * The render depth is a scale from 0 (closest) to 10000 (furthest).
	 * 
	 * \param renderDepth: the new render depth.
	 * If this is -1, this transform will use its parent's render depth.
	 * If it additionally has no parent, it will use render depth 0 (front).
	 */
	void setRenderDepth(float renderDepth);

	/**
	 * Get this transform's render depth.
	 * 
	 * \returns this transform's render depth.
	 */
	float getRenderDepth();

	/**
	 * Get the children of this transform.
	 *
	 * \returns the children of this transform.
	 */
	std::vector<Transform*>& getChildren();

	/**
	 * Get all components attached to this transform.
	 * 
	 * \returns all components attached to this transform.
	 */
	std::vector<std::shared_ptr<Component>>& getComponents();

	/**
	 * Get the rigidbody attached to this transform. Returns nullptr if no rigidbody is attached.
	 * 
	 * \returns the attached rigidbody or nullptr if no rigidbody is attached.
	 */
	RigidbodyPointer getAttachedRigidbody();

	/**
	 * Get a matrix which correctly transforms world space points to local space.
	 * 
	 * \returns the model matrix.
	 */
	glm::mat4 getModelMatrix();

	/**
	 * Get a matrix which correctly transforms parent space points to local space.
	 *
	 * \returns the transform matrix.
	 */
	glm::mat4 getTransformMatrix();

	/**
	 * Get a matrix which correctly transforms local space points to parent space.
	 *
	 * \returns the inverse transform matrix.
	 */
	glm::mat4 getInverseTransformMatrix();

	/**
	 * Transform a point from world space into local space.
	 * 
	 * \param point: the world point to get the local position for.
	 * \return the local position that corresponds to the given world point.
	 */
	glm::vec2 worldPointToLocalPoint(glm::vec2 point);

	/**
	 * Transform a point from world space into the parent's local space.
	 *
	 * \param point: the world point to get the parent's local position for.
	 * \return the parent's local position that corresponds to the given world point.
	 */
	glm::vec2 worldPointToParentLocalPoint(glm::vec2 point);

	/**
	 * Transform a point from local space into world space.
	 *
	 * \param point: the local point to get the world position for.
	 * \return the world position that corresponds to the given local point.
	 */
	glm::vec2 localPointToWorldPoint(glm::vec2 point);

	/**
	 * Get a component on this transform of type T.
	 * 
	 * \returns the first components on this transform with the given type.
	 */
	template <class T>
	std::shared_ptr<Component> getComponent()
	{
		for (std::shared_ptr<Component>& component : components)
		{
			if (typeid(*component) == typeid(T))
			{
				return components[0];
			}
		}
		return NULL;
	}

	/**
	 * Add a component to this transform.
	 * 
	 * \param component: the component to add to this transform.
	 */
	void addComponent(std::shared_ptr<Component> component);

	/**
	 * Remove a specific component from this transform.
	 * 
	 * \param component: the component to remove.
	 * \returns whether a component was succesfully found and removed.
	 */
	bool removeComponent(std::shared_ptr<Component> component);

	/**
	 * Remove a specific component from this transform.
	 *
	 * \param component: the component to remove.
	 * \returns whether a component was succesfully found and removed.
	 */
	bool removeComponent(ComponentPointer component);

	/**
	 * Remove a component of a specific type from this transform.
	 * 
	 * \returns whether a component was succesfully found and removed.
	 */
	template <class T>
	void removeComponentOfType()
	{
		for (unsigned int i{ 0 }; i < components.size(); i++)
		{
			if (typeid(components[i]) == typeid(T))
			{
				components.erase(components.begin() + i);
				return true;
			}
		}

		return false;
	}

	/**
	 * Add a component of type T to this transform.
	 */
	/*
	template <class T>
	std::shared_ptr<T> addComponent()
	{
		static_assert(std::is_base_of<Component, T>::value, "Cannot add a non-component to a transform.");
		std::shared_ptr<T> ptr = T::create();
		addComponent(ptr);
		return ptr;
	}*/

	/**
	 * Get the angle required for this transform to look at a point.
	 * Will NOT set the angle, only calculate it.
	 * Example of using this function to make an object look at the mouse:
	 * \code{.cpp}
	 * transform->rotation = transform->lookAt(Input::getMouseWorldPosition());
	 * \endcode
	 * 
	 * \param point: the point to look at.
	 * \returns the angle at which this transform would look at the point.
	 */
	float lookAt(glm::vec2 point);




	/* GETTERS AND SETTERS */


	/**
	 * Get this transform's world position (relative to origin).
	 * 
	 * \returns the position of this transform.
	 */
	glm::vec2 getPosition();
	/**
	 * Get this transform's position.
	 *
	 * \returns the position of this transform.
	 */
	glm::vec2 getLocalPosition();
	/**
	 * Set this transform's world position.
	 *
	 * \param position: the new world position of this transform.
	 */
	void setPosition(glm::vec2 position);
	/**
	 * Set this transform's local position (relative to parent).
	 *
	 * \param position: the new position of this transform.
	 */
	void setLocalPosition(glm::vec2 position);
	/**
	 * Move this transform in world space. Adds the offset to the world position.
	 *
	 * \param offset: the offset to add to the world position.
	 */
	void movePosition(glm::vec2 offset);
	/**
	 * Move this transform. Adds the offset to the local position (relative to parent).
	 *
	 * \param offset: the offset to add to the local position.
	 */
	void moveLocalPosition(glm::vec2 offset);


	/**
	 * Get this transform's world rotation (relative to origin).
	 *
	 * \returns the rotation of this transform.
	 */
	float getRotation();
	/**
	 * Get this transform's local rotation (relative to parent).
	 *
	 * \returns the rotation of this transform.
	 */
	float getLocalRotation();
	/**
	 * Set this transform's world rotation (relative to origin).
	 *
	 * \param rotation: the new rotation of this transform.
	 */
	void setRotation(float rotation);
	/**
	 * Set this transform's local rotation (relative to parent).
	 *
	 * \param rotation: the new rotation of this transform.
	 */
	void setLocalRotation(float rotation);
	/**
	 * Rotate this transform. Adds the angle to the rotation.
	 *
	 * \param angle: the angle to add to the rotation.
	 */
	void rotate(float angle);

	/**
	 * Get this transform's scale.
	 *
	 * \returns the scale of this transform.
	 */
	glm::vec2 getScale();
	/**
	 * Set this transform's scale.
	 *
	 * \param scale: the new scale of this transform.
	 */
	void setScale(glm::vec2 scale);
	/**
	 * Set this transform's scale.
	 *
	 * \param scale: the new scale of this transform (scale, scale).
	 */
	void setScale(float scale);

private:

	glm::vec2 position;
	float rotation;
	glm::vec2 scale;

	std::string name{};

	float renderDepth{ -1.0f };

	// Flag that when set, causes the transform matrices to be updated
	bool transformUpdated{ true };
	glm::mat4 transform{ glm::identity<glm::mat4>() };
	glm::mat4 inverseTransform{ glm::identity<glm::mat4>() };

	Transform(glm::vec2 position, float rotation, glm::vec2 scale, float renderDepth);

	void updateTransformMatrices();

	std::vector<std::shared_ptr<Component>> components;
	RigidbodyPointer attachedRigidbody{ nullptr };

	Transform* parent = nullptr;
	std::vector<Transform*> children;
};
