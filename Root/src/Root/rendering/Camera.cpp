#include "Camera.h"

Camera::Camera(float cameraHeight)
	: cameraHeight(cameraHeight)
{
	// If no camera was active, set this camera to be active
	if (RootEngine::getActiveCamera() == nullptr)
		setAsActiveCamera();
}

Camera::~Camera()
{
	// If this was the active camera, we must tell Root no camera is active.
	if (RootEngine::getActiveCamera() == this)
		RootEngine::setActiveCamera(nullptr);

	Logger::destructorMessage("Camera");
}

std::shared_ptr<Camera> Camera::create(TransformPointer transform, float cameraHeight)
{
	Camera* camera = new Camera(cameraHeight);
	std::shared_ptr<Camera> pointer{ camera };
	transform->addComponent(pointer);
	return pointer;
}

std::string Camera::toString()
{
	std::stringstream ss;

	// Writing this object to the stream
	ss << "[Camera]"
		<< " > height: " << cameraHeight
		<< std::endl;
	return ss.str();
}

glm::mat4 Camera::getProjectionMatrix()
{
	float aspectRatio{ (float)RootEngine::getScreenWidth() / (float)RootEngine::getScreenHeight() };
	// The projection matrix maps [-aspectRatio, aspectRatio] to [-1, 1] horizontally,
	// and [-1, 1] to [-1, 1] vertically
	return glm::ortho(
		-aspectRatio * (cameraHeight / 2.0f), 
		aspectRatio * (cameraHeight / 2.0f),
		-1.0f * (cameraHeight / 2.0f), 
		1.0f * (cameraHeight / 2.0f), 
		-1.0f, 10000.0f);
}

void Camera::setAsActiveCamera()
{
	RootEngine::setActiveCamera(this);
}

glm::vec2 Camera::worldToScreenPosition(glm::vec2 worldPosition)
{
	if (this == nullptr)
		return glm::vec2(0.0f);

	return glm::vec2(
		glm::vec4(worldPosition.x, worldPosition.y, 0.0f, 1.0f)
		* this->transform->getInverseTransformMatrix()
		* this->getProjectionMatrix()
	);
}

glm::vec2 Camera::screenToWorldPosition(glm::vec2 screenPosition)
{
	if (this == nullptr)
		return glm::vec2(0.0f);

	// First converting screenPosition from [0, width], [0, height] to [-1, 1], [-1, 1]
	screenPosition = glm::vec2(
		(screenPosition.x / (float)RootEngine::getScreenWidth()) * 2.0f - 1.0f,
		(screenPosition.y / (float)RootEngine::getScreenHeight()) * 2.0f - 1.0f
	);

	// Invering vertically
	screenPosition.y *= -1;

	// Then convering to world position
	float aspectRatio{ (float)RootEngine::getScreenWidth() / (float)RootEngine::getScreenHeight() };
	screenPosition.x *= aspectRatio;
	glm::vec2 worldPosition = screenPosition * (cameraHeight / 2.0f);

	worldPosition = glm::vec2(this->transform->getTransformMatrix()
		* glm::vec4(worldPosition.x, worldPosition.y, 0.0f, 1.0f));

	return worldPosition;
}
