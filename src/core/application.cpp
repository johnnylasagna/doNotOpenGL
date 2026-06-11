#include "core/application.h"
#include "core/window.h"
#include "core/input.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>

namespace DoNotOpenGL {

Application::Application(Editor &editor) : window(nullptr), editor(editor) {}

Application::~Application() {
	destroyImGui();
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
	initImGui();

	ma_engine_init(NULL, &engine);

	ma_sound_init_from_file(&engine, "assets/alien.mp3", 0, NULL, NULL, &music);

	return true;
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
		editorModeInput();
		videoModeInput();

		if (editorMode == EditorMode::Video || editorMode == EditorMode::Edit) {
			editor.changeScene();

			if (editor.currentScene != editor.oldScene) {
				activeScene = editor.getScene(editor.currentScene);
			}
		}

		if (editorMode == EditorMode::PlayGround) {
			sceneChangeInput();
		}

		if (playing) {
			if (editorMode == EditorMode::Video || editorMode == EditorMode::Edit) {
				editor.updateTimeline(deltaTime);
			}

			if (activeScene) {
				activeScene->processInput(input, deltaTime);
				activeScene->update(deltaTime);
			}
		}

		input.resetOffsets();

		if (!activeScene) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		} else {
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			if (editorMode == EditorMode::Edit) {
				glViewport(width - videoWidth - 100, height - videoHeight - 100, videoWidth, videoHeight);
			} else {
				glViewport(0, 0, width, height);
			}
			float currentAspectRatio = (height > 0) ? (static_cast<float>(width) / static_cast<float>(height)) : 1.0f;
			activeScene->render(activeScene->getCamera(), currentAspectRatio);
		}

		renderImGui();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}

void Application::initImGui() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void Application::renderImGui() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (editorMode == EditorMode::PlayGround) {
		if (activeScene) {
			activeScene->renderUI();
		}
	}

	if (editorMode == EditorMode::Video || editorMode == EditorMode::Edit) {
		renderPlaybackUI();
	}

	if (editorMode == EditorMode::Edit) {
		renderClipsUI();
	}

	if (editorMode == EditorMode::Edit) {
		ImDrawList *drawList = ImGui::GetBackgroundDrawList();

		float x = screenWidth - videoWidth - 100;
		float y = 100;
		drawList->AddRect(ImVec2(x, y), ImVec2(x + videoWidth, y + videoHeight), IM_COL32(255, 255, 255, 255), 0.0f, 0, 2.0f);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::renderPlaybackUI() {

	ImGui::SetNextWindowPos(ImVec2(screenWidth - videoWidth / 2 - 100, videoHeight + 200), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Appearing);

	ImGui::Begin("Playback");

	if (ImGui::Checkbox("Playing", &playing)) {
		if (playing) {
			editor.changeScene();

			activeScene = editor.getScene(editor.currentScene);

			if (activeScene) {

				activeScene->reset();
				
				activeScene->timeElapsed = 0;

				float timeElapsed = editor.timeElapsedForCurrentScene();

				static float iterationTime = 1.0f / 120.0f;

				float iterations = timeElapsed * 120.0f;

				for (int i = 0; i < iterations; i++) {
					activeScene->update(iterationTime);
				}

				activeScene->timeElapsed = timeElapsed;
			}
			if (editorMode != EditorMode::PlayGround) {
				ma_sound_start(&music);
			}
		} else {
			ma_sound_stop(&music);
		}
	}
	if (ImGui::SliderFloat("Timeline", &editor.timeline, 0.0f, 300.0f)) {
		editor.changeScene();

		activeScene = editor.getScene(editor.currentScene);

		if (activeScene) {

			activeScene->reset();

			activeScene->timeElapsed = 0;

			float timeElapsed = editor.timeElapsedForCurrentScene();

			static float iterationTime = 1.0f / 120.0f;

			float iterations = timeElapsed * 120.0f;

			for (int i = 0; i < iterations; i++) {
				activeScene->update(iterationTime);
			}

			activeScene->timeElapsed = timeElapsed;
		}

		ma_sound_seek_to_second(&music, editor.timeline);

		if (playing && editorMode != EditorMode::PlayGround) {
			ma_sound_start(&music);
		} else {
			ma_sound_stop(&music);
		}
	}

	ImGui::End();
}

void Application::renderClipsUI() { editor.renderClipSettings(); }

void Application::destroyImGui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

Input &Application::getInput() { return input; }

std::shared_ptr<Scene> Application::getActiveScene() { return activeScene; }

ResourceManager &Application::getResourceManager() { return resourceManager; }

void Application::sceneChangeInput() {
	if (input.isKeyPressed(GLFW_KEY_LEFT_SHIFT) && input.isKeyPressed(GLFW_KEY_0)) {
		this->activeScene = editor.getScene(0);
	}
	if (input.isKeyPressed(GLFW_KEY_LEFT_SHIFT) && input.isKeyPressed(GLFW_KEY_1)) {
		this->activeScene = editor.getScene(1);
	}
	if (input.isKeyPressed(GLFW_KEY_LEFT_SHIFT) && input.isKeyPressed(GLFW_KEY_2)) {
		this->activeScene = editor.getScene(2);
	}
	if (input.isKeyPressed(GLFW_KEY_LEFT_SHIFT) && input.isKeyPressed(GLFW_KEY_3)) {
		this->activeScene = editor.getScene(3);
	}
}

void Application::editorModeInput() {
	if (input.isKeyPressed(GLFW_KEY_P)) {
		editorMode = EditorMode::PlayGround;
		ma_sound_stop(&music);
	}
	if (input.isKeyPressed(GLFW_KEY_E)) {
		editorMode = EditorMode::Edit;
		if (playing) {
			ma_sound_start(&music);
		}
	}
	if (input.isKeyPressed(GLFW_KEY_V)) {
		editorMode = EditorMode::Video;
		if (playing) {
			ma_sound_start(&music);
		}
	}
}

void Application::videoModeInput() {
	static bool videoModeChanged = false;

	if (input.isKeyPressed(GLFW_KEY_K)) {
		if (!videoModeChanged) {
			if (playing) {
				playing = false;
				ma_sound_stop(&music);
			} else {
				playing = true;
				if (editorMode != EditorMode::PlayGround) {
					ma_sound_start(&music);
				}
			}
			videoModeChanged = true;
		}
	} else {
		videoModeChanged = false;
	}
}

} // namespace DoNotOpenGL