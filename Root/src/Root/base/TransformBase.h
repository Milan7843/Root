#pragma once

#include "Root/Logger.h"

#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <vector>
#include <typeinfo>
#include <typeindex>

/**
 * Class which holds values for a 2D transformation.
 */
class TransformBase
{
public:

	/**
	 * Destructor for the TransformBase class.
	 */
	~TransformBase();

	/**
	 * Destroy this transform.
	 * Will invalidate any pointers to this transform.
	 * Will also destroy all children, but not the parent.
	 * Any attached components without a reference will also be destroyed.
	 */
	virtual void destroy();

	/**
	 * Get a string representing this Transform.
	 *
	 * \returns this TransformBase in a human-readable format.
	 */
	virtual std::string toString();

	/**
	 * Set the parent transform of this transform.
	 * Having a parent will make the child transform's data in local space.
	 * This will also add this transform as a child to the parent.
	 * Note that adding a child to a transform via Transform::addChild()
	 * will also set the child's parent using setParent(),
	 * and therefore does not have to be called manually.
	 *
	 * \param parent: a shared_ptr to the transform that will be the parent.
	 * If this is NULL, the transform will have no parent.
	 */
	void setParent(TransformBase* parent, bool alsoAddChild = true);

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
	 * Set this transform's tag.
	 * Any transform's tag starts out as empty ("").
	 *
	 * \param tag: the new tag.
	 */
	void setTag(std::string tag);

	/**
	 * Get this transform's tag.
	 * Any transform's tag starts out as empty ("").
	 *
	 * \returns tag: the tag.
	 */
	std::string getTag();

	/**
	 * Get the parent of this transform.
	 *
	 * \returns the parent of this transform, or nullptr if it does not have one.
	 */
	virtual TransformBase* getParent();

	/**
	 * Add a child to this transform.
	 * This will also set the child's parent pointer to this transform.
	 * Note that setting a transform's parent via Transform::setParent()
	 * will also set the parent's child using addChild(),
	 * and therefore does not have to be called manually.
	 *
	 * \param child: a pointer to the child.
	 */
	void addChild(TransformBase* child, bool alsoSetParent = true);

	/**
	 * Remove a child from this transform with the given shared_ptr.
	 * This will also set the child's parent pointer to NULL, indicating no parent.
	 *
	 * \param childToRemove: a pointer to the child that should be removed.
	 * \returns whether a child was removed succesfully.
	 */
	bool removeChild(TransformBase* childToRemove);

	/**
	 * Remove all children from this transform with the given shared_ptr.
	 */
	void removeAllChildren();

	/**
	 * Get the children of this transform.
	 *
	 * \returns the children of this transform.
	 */
	std::vector<TransformBase*>& getChildren();

	/**
	 * Get a normalized vector which points up.
	 *
	 * \returns the up vector.
	 */
	glm::vec2 getLocalUpVector();

	/**
	 * Get a normalized vector which points right.
	 *
	 * \returns the right vector.
	 */
	glm::vec2 getLocalRightVector();

	/**
	 * Get a matrix which correctly transforms world space points to local space.
	 *
	 * \returns the model matrix.
	 */
	glm::mat4 getModelMatrix();

	/**
	 * Get a matrix which correctly transforms world space points to local space,
	 * except for scaling.
	 *
	 * \returns the non-scaled model matrix.
	 */
	glm::mat4 getModelMatrixWithoutScale();

	/**
	 * Get a matrix which correctly transforms parent space points to local space.
	 *
	 * \returns the transform matrix.
	 */
	glm::mat4 getTransformMatrix();

	/**
	 * Get a matrix which correctly transforms parent space points to local space,
	 * except for scaling.
	 *
	 * \returns the non-scaled transform matrix.
	 */
	glm::mat4 getTransformMatrixWithoutScale();

	/**
	 * Get a matrix which correctly transforms local space points to parent space.
	 *
	 * \returns the inverse transform matrix.
	 */
	glm::mat4 getInverseTransformMatrix();

	/**
	 * Get a matrix which correctly transforms local space points to parent space,
	 * except for scaling.
	 *
	 * \returns the non-scaled inverse transform matrix.
	 */
	glm::mat4 getInverseTransformMatrixWithoutScale();

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
	virtual void setPosition(glm::vec2 position);
	/**
	 * Set this transform's local position (relative to parent).
	 *
	 * \param position: the new position of this transform.
	 */
	virtual void setLocalPosition(glm::vec2 position);
	/**
	 * Move this transform in world space. Adds the offset to the world position.
	 *
	 * \param offset: the offset to add to the world position.
	 */
	virtual void movePosition(glm::vec2 offset);
	/**
	 * Move this transform. Adds the offset to the local position (relative to parent).
	 *
	 * \param offset: the offset to add to the local position.
	 */
	virtual void moveLocalPosition(glm::vec2 offset);


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

protected:

	glm::vec2 position;
	float rotation;
	glm::vec2 scale;

	std::string name{};
	std::string tag{};

	// Flag that when set, causes the transform matrices to be updated
	bool transformUpdated{ true };
	glm::mat4 transform{ glm::identity<glm::mat4>() };
	glm::mat4 transformWithoutScale{ glm::identity<glm::mat4>() };
	glm::mat4 inverseTransform{ glm::identity<glm::mat4>() };
	glm::mat4 inverseTransformWithoutScale{ glm::identity<glm::mat4>() };

	TransformBase(glm::vec2 position, float rotation, glm::vec2 scale);

	virtual void updateTransformMatrices();

	TransformBase* parent = nullptr;
	std::vector<TransformBase*> children;
};
