#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "graphics/camera.h"
#include "core/timer.h"
#include "core/input.h"
#include "scenes/scene.h"
#include "core/resourceManager.h"

namespace DoNotOpenGL {

class Application {
  public:
	Application();
	~Application();

	bool init();
	void run();
	void setScene(std::unique_ptr<Scene> newScene);

	Input &getInput();
	Scene *getActiveScene();
	ResourceManager &getResourceManager();

	void initImGui();
	void renderImGui();
	void destroyImGui();

  private:
	GLFWwindow *window = nullptr;
	Timer timer;
	Input input;
	std::unique_ptr<Scene> activeScene;
	ResourceManager resourceManager;
};

} // namespace DoNotOpenGL