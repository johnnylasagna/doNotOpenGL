#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace DoNotOpenGL {

class Input {
  public:
	Input();

	void setWindow(GLFWwindow *window);
	bool isKeyPressed(int key) const;

	// Handle global engine input (Escape to quit, Tab to toggle cursor)
	void processEngineInput();

	// Mouse callback processors
	void processMouseMovement(double xpos, double ypos);
	void processMouseScroll(double yoffset);

	// Getters for the scene to read
	float getMouseOffsetX() const;
	float getMouseOffsetY() const;
	float getScrollOffsetY() const;
	bool getIsCameraActive() const;

	// Reset mouse deltas at the end of the frame
	void resetOffsets();

  private:
	GLFWwindow *window;

	float lastX;
	float lastY;
	float mouseOffsetX;
	float mouseOffsetY;
	float scrollOffsetY;

	bool firstMouse;
	bool isCameraActive;
	bool tabKeyPressed;
};

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

} // namespace DoNotOpenGL