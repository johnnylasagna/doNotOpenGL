#include "graphics/camera.h"

namespace DoNotOpenGL {

Camera::Camera(float yaw, float pitch, float speed, float mouseSensitivity, float zoom)
    : yaw(yaw), pitch(pitch), speed(speed), mouseSensitivity(mouseSensitivity), zoom(zoom) {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() { return glm::lookAt(pos, pos + front, up); }

void Camera::updateCameraVectors() {

	glm::vec3 direction;

	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));

	direction.y = sin(glm::radians(pitch));

	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	front = glm::normalize(direction);

	right = glm::normalize(glm::cross(front, worldUp));

	up = glm::normalize(glm::cross(right, front));
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {

	float velocity = speed * deltaTime;

	if (direction == CameraMovement::FORWARD) {
		pos += front * velocity;
	}

	if (direction == CameraMovement::BACKWARD) {
		pos -= front * velocity;
	}

	if (direction == CameraMovement::LEFT) {
		pos -= right * velocity;
	}

	if (direction == CameraMovement::RIGHT) {
		pos += right * velocity;
	}
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {

	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (constrainPitch) {
		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		if (pitch < -89.0f) {
			pitch = -89.0f;
		}
	}

	updateCameraVectors();
}
void Camera::ProcessMouseScroll(float yoffset) {
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

void Camera::moveToward(float deltaTime, float desiredDistance) {
	glm::vec3 toTarget = target - pos;
	float currentDistance = glm::length(toTarget);
	if (currentDistance > desiredDistance) {
		pos += glm::normalize(toTarget) * speed * deltaTime;
	}
}

glm::mat4 Camera::lookAtTarget() {
	return glm::lookAt(pos, target, worldUp);
}

} // namespace DoNotOpenGL