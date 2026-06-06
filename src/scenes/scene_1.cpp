#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "scenes/scene_1.h"
#include "core/application.h"
#include <numeric>

namespace DoNotOpenGL {

Scene1::Scene1(Application *app) : Scene(app), camera(0.0f, 0.0f, 5.0f, 0.1f, 45.0f) {}

void Scene1::init() {

	camera.pos = glm::vec3(-100.0f, 0.0f, 0.0f);

	ResourceManager &rm = application->getResourceManager();

	Shader *lightShader = rm.loadShader("light", "shaders/vertex/light.vs", "shaders/fragment/light.fs");
	Shader *objectShader = rm.loadShader("object", "shaders/vertex/object.vs", "shaders/fragment/object.fs");

	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

	objectShader->use();
	objectShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	objectShader->setVec3("objectColor", 0.2f, 0.5f, 0.5f);
	objectShader->setVec3("lightPos", lightPos);

	lightShader->use();
	lightShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	Texture *objectTexture = rm.loadTexture("planet", "assets/earth_day.jpg");
	Texture *lightTexture = rm.loadTexture("sun", "assets/sun.jpg");

	float theta = 0;
	float phi = 0;
	float radius = 5;

	std::vector<float> vertices;

	float PI = 3.1415927;

	int stacks = 300; // divisions along theta (0 to PI)
	int slices = 300; // divisions along phi (0 to 2*PI)

	for (int i = 0; i <= stacks; i++) {
		float theta = i * PI / stacks; // 0 to PI

		for (int j = 0; j <= slices; j++) {
			float phi = j * 2.0f * PI / slices; // 0 to 2*PI

			float x = radius * sin(theta) * cos(phi);
			float y = radius * sin(theta) * sin(phi);
			float z = radius * cos(theta);

			// Position
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);

			// Normal (same as position for unit sphere)
			vertices.push_back(x / radius);
			vertices.push_back(y / radius);
			vertices.push_back(z / radius);

			// Texture coordinates
			vertices.push_back((float)j / slices);
			vertices.push_back((float)i / stacks);
		}
	}

	std::vector<unsigned int> indices;

	for (int i = 0; i < stacks; i++) {
		for (int j = 0; j < slices; j++) {
			int row1 = i * (slices + 1);
			int row2 = (i + 1) * (slices + 1);

			// Triangle 1
			indices.push_back(row1 + j);
			indices.push_back(row2 + j);
			indices.push_back(row1 + j + 1);

			// Triangle 2
			indices.push_back(row1 + j + 1);
			indices.push_back(row2 + j);
			indices.push_back(row2 + j + 1);
		}
	}

	std::unique_ptr<Mesh> lightMesh = std::make_unique<Mesh>(vertices, indices);
	std::unique_ptr<Mesh> objectMesh = std::make_unique<Mesh>(vertices, indices);

	Object lightObject(std::move(lightMesh), lightShader, std::vector<Texture *>{lightTexture});
	Object objectObject(std::move(objectMesh), objectShader, std::vector<Texture *>{objectTexture});

	lightObject.setPosition(0.0f, 0.0f, 0.0f);
	lightObject.setScale(0.5f, 0.5f, 0.5f);
	lightObject.setRotation(0, 0.0f, 1.0f, 0.0f);

	objectObject.setPosition(10.0f, 0.0f, 0.0f);
	objectObject.setScale(0.25f, 0.25f, 0.25f);
	objectObject.setRotation(0, 0.0f, 0.1f, 0.0f);

	objects.push_back(std::move(lightObject));
	objects.push_back(std::move(objectObject));
}

void Scene1::update(float deltaTime) {

	static float theta = 0.0f;

	objects[1].setPosition(25 * cos(theta), 0.0f, 25 * sin(theta));
	objects[1].setRotation(theta, 0.0f, 1.0f, 0.0f);
	objects[0].setRotation(theta / 10, 0.0f, 1.0f, 0.0f);

	theta += 1.0f * deltaTime;

	camera.target = objects[1].getPosition();
	camera.moveToward(deltaTime, 1.0f);
}

void Scene1::render(Camera &camera, float aspectRatio) {
	// glClearColor(0.4f, 0.0f, 0.6f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	static double PI = 3.1415927;
	static float theta = 0;

	if (aspectRatio <= 0.0f) {
		aspectRatio = 0.1f;
	}

	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), aspectRatio, 0.1f, 100.0f);
	glm::mat4 view = camera.lookAtTarget();
	// glm::mat4 view = camera.GetViewMatrix();

	for (auto &object : objects) {
		object.setProjection(projection);
		object.setView(view);
		object.setModel();
		object.render();
	}
}

void Scene1::processInput(Input &input, float deltaTime) {
	if (!input.getIsCameraActive()) {
		return;
	}

	if (input.isKeyPressed(GLFW_KEY_W)) {
		camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
	}
	if (input.isKeyPressed(GLFW_KEY_S)) {
		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
	}
	if (input.isKeyPressed(GLFW_KEY_A)) {
		camera.processKeyboard(CameraMovement::LEFT, deltaTime);
	}
	if (input.isKeyPressed(GLFW_KEY_D)) {
		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
	}

	float xOffset = input.getMouseOffsetX();
	float yOffset = input.getMouseOffsetY();
	if (xOffset != 0.0f || yOffset != 0.0f) {
		camera.processMouseMovement(xOffset, yOffset);
	}

	float scrollOffset = input.getScrollOffsetY();
	if (scrollOffset != 0.0f) {
		camera.ProcessMouseScroll(scrollOffset);
	}
}

Camera &Scene1::getCamera() { return camera; }

} // namespace DoNotOpenGL