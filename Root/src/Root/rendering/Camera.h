#pragma once

#include "Root/components/Component.h"
#include "Root/Logger.h"
#include "Root/engine/RootEngine.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#define CameraPointer std::shared_ptr<Camera>

/**
 * Class that holds information about a camera in a component form.
 */
class Camera : public Component
{
public:

	/**
	 * Create a new Camera.
	 * Will automatically set the active camera to this if no camera was active.
	 *
	 * \param cameraHeight: the height of the camera [optional: default = 2]
	 */
	Camera(float cameraHeight = 2.0f); // Initialize cameraHeight to 2.0

	~Camera();

	/**
	 * Create a new Camera.
	 * Will automatically set the active camera to this if no camera was active.
	 * Will automatically add this component to the given transform.
	 *
	 * \param transform: the transform to add this component to.
	 * \param cameraHeight: the height of the camera [optional: default = 2]
	 */
	static std::shared_ptr<Camera> create(TransformPointer transform, float cameraHeight = 2.0f);

	/**
	 * Get a string representing this Component.
	 * 
	 * \returns this Component in a human-readable format.
	 */
	std::string toString() override;

	/**
	 * Get the projection matrix which belongs to the values of this camera.
	 * 
	 * \return the projection matrix for this camera
	 */
	glm::mat4 getProjectionMatrix();

	/**
	 * Set this camera as the current active one used for rendering.
	 */
	void setAsActiveCamera();


	/**
	 * Get the screen position corresponding to the given world position.
	 *
	 * \param worldPosition: the world position to convert to screen space.
	 * \returns the screen position corresponding to the given world position.
	 */
	glm::vec2 worldToScreenPosition(glm::vec2 worldPosition);

	/**
	 * Get the world position corresponding to the given screen position.
	 *
	 * \param screenPosition: the screen position to convert to world space.
	 * \returns the world position corresponding to the given screen position.
	 */
	glm::vec2 screenToWorldPosition(glm::vec2 screenPosition);

private:

	float cameraHeight;
};

