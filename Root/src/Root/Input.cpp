#include "Input.h"

#include "Root/engine/InputEngine.h"

namespace Input
{
	bool getKey(int key)
	{
		return InputEngine::getKey(key);
	}

	bool getKeyPressed(int key)
	{
		return InputEngine::getKeyPressed(key);
	}
	bool getKeyReleased(int key)
	{
		return InputEngine::getKeyReleased(key);
	}

	bool getMouseButton(int mouseButton)
	{
		return InputEngine::getMouseButton(mouseButton);
	}

	bool getMouseButtonPressed(int mouseButton)
	{
		return InputEngine::getMouseButtonPressed(mouseButton);
	}

	bool getMouseButtonReleased(int mouseButton)
	{
		return InputEngine::getMouseButtonReleased(mouseButton);
	}

	glm::vec2 getMousePosition()
	{
		return InputEngine::getMousePosition();
	}

	glm::vec2 getMouseDelta()
	{
		return InputEngine::getMouseDelta();
	}

	glm::vec2 getMouseWorldPosition()
	{
		return InputEngine::getMouseWorldPosition();
	}
};
