#pragma once

#define ComponentPointer Component*

#include <string>

class Transform;

/**
 * Class which all components must inherit from.
 */
class Component
{
public:

	/**
	 * The start function of this component, gets called automatically at the start of the game.
	 */
	virtual void start() {}

	/**
	 * The update function of this component, gets called automatically every frame.
	 */
	virtual void update() {}

	/**
	 * The render function of this component, gets called automatically every frame.
	 * This function gets called after update().
	 * 
	 * \param renderDepth: the depth at which this component should be rendered.
	 */
	virtual void render(float renderDepth) {}

	/**
	 * Set the pointer to the transform of this component.
	 * 
	 * \param transform: a pointer to the transform which this component belongs to.
	 */
	void setTransform(Transform* transform);

	/**
	 * Get the pointer to the transform which this component belongs to.
	 *
	 * \returns the pointer to the transform which this component belongs to.
	 */
	Transform* getTransform();

	/**
	 * Get a string representing this Component.
	 *
	 * \returns this Component in a human-readable format.
	 */
	virtual std::string toString() { return std::string{}; };

	bool internal_started();

	void internal_start();

protected:

	// A pointer to the transform this component belongs to.
	Transform* transform;

	// Holds whether the start method has been called or not
	bool internal_called_start{ false };
};
