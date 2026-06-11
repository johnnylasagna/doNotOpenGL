#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "miniaudio/miniaudio.h"

#include "graphics/camera.h"
#include "core/timer.h"
#include "core/input.h"
#include "scenes/scene.h"
#include "core/resourceManager.h"
#include "core/editor.h"

namespace DoNotOpenGL {

enum class EditorMode { Video, PlayGround, Edit };

class Application {
  public:
	Application(Editor &editor);
	~Application();

	bool init();
	void run();

	Input &getInput();
	std::shared_ptr<Scene> getActiveScene();
	ResourceManager &getResourceManager();

	EditorMode editorMode = EditorMode::PlayGround;

	void initImGui();
	void renderImGui();
	void destroyImGui();

  private:
	GLFWwindow *window = nullptr;
	Timer timer;
	Input input;
	std::shared_ptr<Scene> activeScene;
	ResourceManager resourceManager;
	Editor &editor;

	void sceneChangeInput();
	void videoModeInput();
	void editorModeInput();

	void renderPlaybackUI();
	void renderClipsUI();

	bool playing = false;

	ma_engine engine;
	ma_sound music;
};

} // namespace DoNotOpenGL