#include "core/input.h"
#include "core/application.h"

namespace DoNotOpenGL {

Input::Input()
    : window(nullptr), lastX(400.0f), lastY(300.0f), mouseOffsetX(0.0f), mouseOffsetY(0.0f), scrollOffsetY(0.0f),
      firstMouse(true), isCameraActive(true), tabKeyPressed(false) {}

void Input::setWindow(GLFWwindow *win) { window = win; }

bool Input::isKeyPressed(int key) const {
	if (!window)
		return false;
	return glfwGetKey(window, key) == GLFW_PRESS;
}

void Input::processEngineInput() {
	if (isKeyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

	if (isKeyPressed(GLFW_KEY_TAB)) {
		if (!tabKeyPressed) {
			isCameraActive = !isCameraActive;

			if (isCameraActive) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				firstMouse = true;
			} else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			tabKeyPressed = true;
		}
	} else {
		tabKeyPressed = false;
	}
}

void Input::processMouseMovement(double xposIn, double yposIn) {
	if (!isCameraActive)
		return;

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	mouseOffsetX += (xpos - lastX);
	mouseOffsetY += (lastY - ypos);

	lastX = xpos;
	lastY = ypos;
}

void Input::processMouseScroll(double yoffset) {
	if (!isCameraActive) {
		return;
	}
	scrollOffsetY += static_cast<float>(yoffset);
}

float Input::getMouseOffsetX() const { return mouseOffsetX; }
float Input::getMouseOffsetY() const { return mouseOffsetY; }
float Input::getScrollOffsetY() const { return scrollOffsetY; }
bool Input::getIsCameraActive() const { return isCameraActive; }

void Input::resetOffsets() {
	mouseOffsetX = 0.0f;
	mouseOffsetY = 0.0f;
	scrollOffsetY = 0.0f;
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
	auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
	if (app != nullptr) {
		app->getInput().processMouseMovement(xposIn, yposIn);
	}
}

void scroll_callback(GLFWwindow *window, double, double yoffset) {
	auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
	if (app != nullptr) {
		app->getInput().processMouseScroll(yoffset);
	}
}

} // namespace DoNotOpenGL