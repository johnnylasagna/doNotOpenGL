#include "core/editor.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace DoNotOpenGL {

Editor::Editor() {}

void Editor::addScene(std::shared_ptr<Scene> scene) {
	scene->init();
	scenes.push_back(scene);
}

void Editor::addClip(int index, float duration) { clips.emplace_back(index, duration); }

void Editor::changeScene() {
	oldScene = currentScene;

	auto it = std::lower_bound(durations.begin(), durations.end(), timeline);
	if (it != durations.end()) {
		int index = it - durations.begin();
		currentScene = clips[index].sceneIndex;
		currentClip = index;
		return;
	}

	currentClip = -1;
	currentScene = -1;
}

std::shared_ptr<Scene> Editor::getScene(int sceneNumber) {
	if (sceneNumber == -1) {
		return nullptr;
	}
	return scenes[sceneNumber];
}

void Editor::updateTimeline(float deltaTime) { timeline += deltaTime; }

float Editor::timeElapsedForCurrentScene() {
	if (currentClip == -1) {
		return -1;
	}

	float clipStart = currentClip == 0 ? 0.0f : durations[currentClip - 1];

	return (timeline - clipStart) + sceneDurations[currentClip];
}

void Editor::resetDurations() {
	float accumulatedTime = 0.0f;

	currMaxDuration.clear();
	durations.clear();
	sceneDurations.clear();

	for (const Clip &clip : clips) {
		accumulatedTime += clip.duration;
		float endTime = accumulatedTime;

		durations.push_back(endTime);
		sceneDurations.push_back(currMaxDuration[clip.sceneIndex]);
		currMaxDuration[clip.sceneIndex] += clip.duration;
	}
}

void Editor::renderClipSettings() {
	bool clipsChanged = false;

	ImGui::Begin("Clip Settings");

	if (ImGui::Button("Add Clip")) {
		clips.push_back({0, 1.0f});
		clipsChanged = true;
	}

	ImGui::Separator();

	int removeIndex = -1;

	float accumulatedTime = 0.0f;

	for (size_t i = 0; i < clips.size(); ++i) {
		Clip &clip = clips[i];

		ImGui::PushID((int)i);

		std::string header = "Clip " + std::to_string(i);

		if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
			float startTime = accumulatedTime;
			float endTime = accumulatedTime + clip.duration;

			ImGui::Text("Range: %.2f -> %.2f", startTime, endTime);

			std::string currentScene = "Scene " + std::to_string(clip.sceneIndex);

			if (ImGui::BeginCombo("Scene", currentScene.c_str())) {
				for (size_t sceneIndex = 0; sceneIndex < scenes.size(); ++sceneIndex) {
					std::string label = "Scene " + std::to_string(sceneIndex);

					bool selected = clip.sceneIndex == (int)sceneIndex;

					if (ImGui::Selectable(label.c_str(), selected)) {
						clip.sceneIndex = (int)sceneIndex;
					}

					if (selected) {
						ImGui::SetItemDefaultFocus();
					}
				}

				ImGui::EndCombo();
			}

			if (ImGui::DragFloat("Duration", &clip.duration, 0.01f, 0.01f, 10000.0f)) {
				clipsChanged = true;
			}

			if (ImGui::Button("Remove")) {
				removeIndex = (int)i;
				clipsChanged = true;
			}
		}

		accumulatedTime += clip.duration;

		ImGui::Separator();

		ImGui::PopID();
	}

	if (removeIndex != -1) {
		clips.erase(clips.begin() + removeIndex);
	}

	ImGui::End();

	if (clipsChanged) {
		resetDurations();
	}
}

} // namespace DoNotOpenGL