#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <chrono> // Time measurement

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

namespace Profiler
{
	struct Checkpoint
	{
		std::chrono::steady_clock::time_point time;
		std::string label;
	};

	extern bool enabled;

	extern unsigned int timeSinceUpdate;
	extern unsigned int framesBetweenUpdates;

	extern std::vector<Checkpoint> currentCheckpoints;

	// Create w little window displaying the information with ImGui
	void createProfilerWindow();

	// Disable the profiler
	void disable();

	// Add a new checkpoint to the current list
	void addCheckpoint(const char* label);

	// Empty the current list of checkpoints
	void emptyCheckpointList();
};

