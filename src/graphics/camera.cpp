#include "graphics/camera.h"

namespace DoNotOpenGL {

Camera::Camera(float yaw, float pitch, float speed, float mouseSensitivity, float zoom)
    : yaw(yaw), pitch(pitch), speed(speed), mouseSensitivity(mouseSensitivity), zoom(zoom) {
	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
	if (targetSet == CameraTarget::UNSET) {
		return glm::lookAt(pos, pos + front, up);
	}
	else {
		return glm::lookAt(pos, target, worldUp);
	}
}

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

	if (mode == CameraMode::FLY) {
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
		if (direction == CameraMovement::UP) {
			pos += up * velocity;
		}
		if (direction == CameraMovement::DOWN) {
			pos -= up * velocity;
		}
	}
	if (mode == CameraMode::CREATIVE) {
		if (direction == CameraMovement::FORWARD) {
			glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
			pos += flatFront * velocity;
		}
		if (direction == CameraMovement::BACKWARD) {
			glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
			pos -= flatFront * velocity;
		}
		if (direction == CameraMovement::LEFT) {
			glm::vec3 flatRight = glm::normalize(glm::vec3(right.x, 0.0f, right.z));
			pos -= flatRight * velocity;
		}
		if (direction == CameraMovement::RIGHT) {
			glm::vec3 flatRight = glm::normalize(glm::vec3(right.x, 0.0f, right.z));
			pos += flatRight * velocity;
		}

		if (direction == CameraMovement::UP) {
			pos += worldUp * velocity;
		}

		if (direction == CameraMovement::DOWN) {
			pos -= worldUp * velocity;
		}
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

} // namespace DoNotOpenGL