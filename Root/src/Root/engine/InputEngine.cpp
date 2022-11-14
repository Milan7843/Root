#include "InputEngine.h"

#include <Root/Input.h>

namespace InputEngine
{
	namespace
	{
		std::vector<int> keysDownThisFrame;
		std::vector<int> keysDownLastFrame;
		glm::vec2 mousePosition;
		glm::vec2 deltaMousePosition;

		const int ALL_KEYS[] {
			KEY_SPACE,
			KEY_APOSTROPHE,
			KEY_COMMA,
			KEY_MINUS,
			KEY_PERIOD,
			KEY_SLASH,
			KEY_0,
			KEY_1,
			KEY_2,
			KEY_3,
			KEY_4,
			KEY_5,
			KEY_6,
			KEY_7,
			KEY_8,
			KEY_9,
			KEY_SEMICOLON,
			KEY_EQUAL,
			KEY_A,
			KEY_B,
			KEY_C,
			KEY_D,
			KEY_E,
			KEY_F,
			KEY_G,
			KEY_H,
			KEY_I,
			KEY_J,
			KEY_K,
			KEY_L,
			KEY_M,
			KEY_N,
			KEY_O,
			KEY_P,
			KEY_Q,
			KEY_R,
			KEY_S,
			KEY_T,
			KEY_U,
			KEY_V,
			KEY_W,
			KEY_X,
			KEY_Y,
			KEY_Z,
			KEY_LEFT_BRACKET,
			KEY_BACKSLASH,
			KEY_RIGHT_BRACKET,
			KEY_GRAVE_ACCENT,
			KEY_WORLD_1,
			KEY_WORLD_2,


			KEY_ESCAPE,
			KEY_ENTER,
			KEY_TAB,
			KEY_BACKSPACE,
			KEY_INSERT,
			KEY_DELETE,
			KEY_RIGHT,
			KEY_LEFT,
			KEY_DOWN,
			KEY_UP,
			KEY_PAGE_UP,
			KEY_PAGE_DOWN,
			KEY_HOME,
			KEY_END,
			KEY_CAPS_LOCK,
			KEY_SCROLL_LOCK,
			KEY_NUM_LOCK,
			KEY_PRINT_SCREEN,
			KEY_PAUSE,
			KEY_F1,
			KEY_F2,
			KEY_F3,
			KEY_F4,
			KEY_F5,
			KEY_F6,
			KEY_F7,
			KEY_F8,
			KEY_F9,
			KEY_F10,
			KEY_F11,
			KEY_F12,
			KEY_F13,
			KEY_F14,
			KEY_F15,
			KEY_F16,
			KEY_F17,
			KEY_F18,
			KEY_F19,
			KEY_F20,
			KEY_F21,
			KEY_F22,
			KEY_F23,
			KEY_F24,
			KEY_F25,


			KEY_KP_0,
			KEY_KP_1,
			KEY_KP_2,
			KEY_KP_3,
			KEY_KP_4,
			KEY_KP_5,
			KEY_KP_6,
			KEY_KP_7,
			KEY_KP_8,
			KEY_KP_9,
			KEY_KP_DECIMAL,
			KEY_KP_DIVIDE,
			KEY_KP_MULTIPLY,
			KEY_KP_SUBTRACT,
			KEY_KP_ADD,
			KEY_KP_ENTER,
			KEY_KP_EQUAL,
			KEY_LEFT_SHIFT,
			KEY_LEFT_CONTROL,
			KEY_LEFT_ALT,
			KEY_LEFT_SUPER,
			KEY_RIGHT_SHIFT,
			KEY_RIGHT_CONTROL,
			KEY_RIGHT_ALT,
			KEY_RIGHT_SUPER,
			KEY_MENU
		};


		const int ALL_MOUSE_BUTTONS[]{
			MOUSE_LEFT,
			MOUSE_RIGHT,
			MOUSE_MIDDLE,
			MOUSE_BUTTON_1,
			MOUSE_BUTTON_2,
			MOUSE_BUTTON_3,
			MOUSE_BUTTON_4,
			MOUSE_BUTTON_5,
			MOUSE_BUTTON_6,
			MOUSE_BUTTON_7,
			MOUSE_BUTTON_8
		};

		bool find(std::vector<int>& v, int s)
		{
			for (int i : v)
			{
				if (i == s)
					return true;
			}
			return false;
		}
	}

	void initialise()
	{
	}

	void update()
	{
		for (int key : ALL_KEYS)
		{
			if (glfwGetKey(RootEngine::getActiveWindow(), key) == GLFW_PRESS)
				keysDownThisFrame.push_back(key);
		}
		for (int button : ALL_MOUSE_BUTTONS)
		{
			if (glfwGetMouseButton(RootEngine::getActiveWindow(), button) == GLFW_PRESS)
				keysDownThisFrame.push_back(button);
		}
		double mouseX, mouseY;
		glfwGetCursorPos(RootEngine::getActiveWindow(), &mouseX, &mouseY);
		mousePosition = glm::vec2(mouseX, mouseY);
	}

	void newFrame()
	{
		keysDownLastFrame = keysDownThisFrame;
		keysDownThisFrame.clear();
	}

	bool getKey(int key)
	{
		return find(keysDownThisFrame, key);
	}

	bool getKeyPressed(int key)
	{
		// Key must be pressed now but not last frame
		return find(keysDownThisFrame, key) && !find(keysDownLastFrame, key);
	}

	bool getKeyReleased(int key)
	{
		// Key must not be pressed now but last frame
		return !find(keysDownThisFrame, key) && find(keysDownLastFrame, key);
	}

	bool getMouseButton(int mouseButton)
	{
		return find(keysDownThisFrame, mouseButton);
	}

	bool getMouseButtonPressed(int mouseButton)
	{
		// Key must be pressed now but not last frame
		return find(keysDownThisFrame, mouseButton) && !find(keysDownLastFrame, mouseButton);
	}

	bool getMouseButtonReleased(int mouseButton)
	{
		// Key must not be pressed now but last frame
		return !find(keysDownThisFrame, mouseButton) && find(keysDownLastFrame, mouseButton);
	}

	glm::vec2 getMousePosition()
	{
		return mousePosition;
	}

	glm::vec2 getMouseDelta()
	{
		return deltaMousePosition;
	}

	glm::vec2 getMouseWorldPosition()
	{
		glm::vec2 screenPos = mousePosition;
		return RootEngine::getActiveCamera()->screenToWorldPosition(screenPos);
	}

	glm::vec2 getMouseNormalizedScreenPosition()
	{
		glm::vec2 screenPosition = mousePosition;

		screenPosition = glm::vec2(
			(screenPosition.x / (float)RootEngine::getScreenWidth()) * 2.0f - 1.0f,
			(screenPosition.y / (float)RootEngine::getScreenHeight()) * 2.0f - 1.0f
		);

		return screenPosition;
	}
};
