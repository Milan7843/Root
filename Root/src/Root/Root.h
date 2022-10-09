#pragma once

#include "Transform.h"
#include "Root/Components.h"
#include "Root/components/Script.h"
#include "shaders/Shader.h"

#include "simpleprofiler/Profiler.h"
#include "Time.h"
#include "rendering/Renderer.h"

#include <simpletext/SimpleText.h>
#include <rootgui/RootGUI.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include "imgui/imgui_stdlib.h"

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


#include "Logger.h"

class Camera;

namespace Root
{
	/**
	* <summary>
	* Start the Root framework.
	* <param> windowWidth the width of the window used to render to.
	* <param> windowHeight the height of the window used to render to.
	* <returns> 0 for succesful run, -1 for error.
	* </summary>
	*/
	int Start(unsigned int windowWidth, unsigned int windowHeight);

	/**
	 * Get a pointer to the shader used for sprite rendering.
	 * 
	 * \returns a pointer to the shader used for sprite rendering.
	 */
	Shader* getSpriteRenderShader();

	/**
	 * Add a script to the scene.
	 * 
	 * \param script: the script to add to the scene.
	 */
	void addScript(std::shared_ptr<Script> script);

	/**
	 * Add a transform to the scene.
	 *
	 * \param transform: the transform to add to the scene.
	 */
	void addTransform(std::shared_ptr<Transform> transform);

	/**
	 * Get a transform by its name.
	 *
	 * \returns a TransformPointer to a transform with the given name if it was found,
	 * nullptr otherwise.
	 */
	TransformPointer getTransformByName(std::string& name);

	/**
	 * Get the camera currently actively used for rendering.
	 *
	 * \returns the currently active camera.
	 */
	Camera* getActiveCamera();


	/**
	 * Get the current width of the screen.
	 */
	unsigned int getScreenWidth();
	/**
	 * Get the current height of the screen.
	 */
	unsigned int getScreenHeight();
};
