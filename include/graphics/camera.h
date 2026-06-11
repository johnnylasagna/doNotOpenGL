#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/settings.h"

namespace DoNotOpenGL {

enum class CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, DOWN, UP };
enum class CameraMode {FLY, CREATIVE};
enum class CameraTarget {UNSET, SET};

struct Camera {
	glm::vec3 pos{};
	glm::vec3 front{};
	glm::vec3 up{};
	glm::vec3 right{};
	glm::vec3 worldUp{};
	glm::vec3 target{};

	float yaw{};
	float pitch{};
	float speed{};
	float mouseSensitivity{};
	float zoom{};

	float previousFrame{};
	float currentFrame{};

	Camera(float yaw = -90.0f, float pitch = 0.0f, float speed = 2.5f, float mouseSensitivity = 1.0f, float zoom = 45.0f);
	CameraMode mode = CameraMode::CREATIVE;
	CameraTarget targetSet = CameraTarget::UNSET;

	glm::mat4 GetViewMatrix();
	void updateCameraVectors();
	void processKeyboard(CameraMovement direction, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	void processMouseScroll(float yoffset);
	void moveToward(float deltaTime, float desiredDistance);
};

} // namespace DoNotOpenGL