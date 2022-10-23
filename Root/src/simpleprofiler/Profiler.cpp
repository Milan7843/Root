#include "Profiler.h"


bool Profiler::enabled{ true };
unsigned int Profiler::timeSinceUpdate{ 0 };
unsigned int Profiler::framesBetweenUpdates{ 10 };

std::vector<Profiler::Checkpoint> Profiler::currentCheckpoints;

void Profiler::createProfilerWindow()
{
	if (!Profiler::enabled)
		return;

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// Creating the GUI window
	ImGui::Begin("Profiler");
	ImGui::SetWindowSize(ImVec2(200.0f, 300.0f));

	if (currentCheckpoints.size() >= 1)
		ImGui::Text(currentCheckpoints[0].label.c_str());

	for (unsigned int i{ 1 }; i < currentCheckpoints.size(); i++)
	{
		auto difference = std::chrono::duration_cast<std::chrono::nanoseconds>(currentCheckpoints[i].time - currentCheckpoints[i-1].time).count();
		std::chrono::duration<double> diff = currentCheckpoints[i].time - currentCheckpoints[i - 1].time;
		ImGui::Text((currentCheckpoints[i].label + ": " + std::to_string(diff.count() * 1000.0f) + "ms").c_str());
	}

	ImGui::End();

	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Profiler::disable()
{
	Profiler::enabled = false;
}

void Profiler::addCheckpoint(const std::string label)
{
	if (!Profiler::enabled)
		return;

	if (timeSinceUpdate != 0)
		return;

	Checkpoint newCheckpoint{ std::chrono::high_resolution_clock::now(), label };
	currentCheckpoints.push_back(newCheckpoint);
}

void Profiler::emptyCheckpointList()
{
	if (!Profiler::enabled)
		return;

	if (timeSinceUpdate < framesBetweenUpdates)
	{
		timeSinceUpdate++;
		return;
	}

	currentCheckpoints.clear();
	timeSinceUpdate = 0;
}
