#pragma once

#include "scenes/scene.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace DoNotOpenGL {

struct Clip {
	int sceneIndex;
	float duration;
};

class Editor {
  private:
	std::vector<std::shared_ptr<Scene>> scenes;
	std::vector<Clip> clips;
	std::vector<float> durations;
	std::vector<float> sceneDurations;
	std::unordered_map<unsigned int, float> currMaxDuration;

  public:
	Editor();
	void addScene(std::shared_ptr<Scene> scene);
	void addClip(int index, float duration);
	void changeScene();
	std::shared_ptr<Scene> getScene(int sceneNumber);

	void resetDurations();

	float timeline = 0.0f;
	int currentScene = -1;
	int oldScene = -1;
	int currentClip = -1;

	void updateTimeline(float deltaTime);
	float timeElapsedForCurrentScene();
	void renderClipSettings();
};

} // namespace DoNotOpenGL