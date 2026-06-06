#include "core/application.h"
#include "core/window.h"
#include "core/input.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"

#include <iostream>

namespace DoNotOpenGL {

Application::Application() : window(nullptr) {}

Application::~Application() {
	resourceManager.clear();
	glfwTerminate();
}

bool Application::init() {
	window = createWindow();
	if (window == nullptr) {
		return false;
	}

	input.setWindow(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetWindowUserPointer(window, this);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	return true;
}

void Application::setScene(std::unique_ptr<Scene> newScene) {
	activeScene = std::move(newScene);
	if (activeScene) {
		activeScene->init();
	}
}

void Application::run() {
	float currentFrame = 0.0f;
	float previousFrame = 0.0f;
	float deltaTime = 0.0f;

	while (!glfwWindowShouldClose(window)) {
		timer.fps();

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - previousFrame;
		previousFrame = currentFrame;

		input.processEngineInput();

		if (activeScene) {
			activeScene->processInput(input, deltaTime);
			activeScene->update(deltaTime);
		}

		input.resetOffsets();

		if (!activeScene) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		} else {
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			float currentAspectRatio = (height > 0) ? (static_cast<float>(width) / static_cast<float>(height)) : 1.0f;
			activeScene->render(activeScene->getCamera(), currentAspectRatio);
		}

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

Input &Application::getInput() { return input; }

Scene *Application::getActiveScene() { return activeScene.get(); }

ResourceManager &Application::getResourceManager() { return resourceManager; }

} // namespace DoNotOpenGL