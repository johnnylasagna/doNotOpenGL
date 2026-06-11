#include "scenes/scene_1.h"
#include "core/application.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <numeric>

namespace DoNotOpenGL {

Scene1::Scene1(Application *app) : Scene(app) {}

void Scene1::initCamera() {
	camera = Camera(180.0f, 0.0f, 5.0f, 0.1f, 45.0f);
	camera.pos = glm::vec3(-100.0f, 0.0f, 0.0f);
	camera.targetSet = CameraTarget::SET;
}

void Scene1::init() {
	speed = 1.0f;

	initCamera();
	initLights();
	initEnvironment();
	initSkybox();
}

void Scene1::reset() {
	camera = Camera(180.0f, 0.0f, 5.0f, 0.1f, 45.0f);
	camera.pos = glm::vec3(-100.0f, 0.0f, 0.0f);
	camera.targetSet = CameraTarget::SET;
	lightObjects[0].transform = Transform{glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f};
	materialObjects[0].transform = Transform{glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.25f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f};
}

void Scene1::initLights() {
	ResourceManager &rm = application->getResourceManager();

	auto sphereMesh = rm.getMesh("sphere", []() { return createSphereMesh(300, 300); });
	auto lightShader = rm.loadShader("sun", "shaders/vertex/uber.vs", "shaders/fragment/uber.fs");
	auto lightTexture = rm.loadTexture("sun", "assets/sun.jpg");

	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);

	Transform lightTransform{lightPos, glm::vec3(2.5f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f};

	Object lightObject(sphereMesh, lightShader, lightTransform);

	lightObject.isLightSource = true;
	lightObject.bodyTexture = lightTexture;
	lightObject.sourceLightColor = glm::vec3(1.0f);

	Light sunLight;
	sunLight.type = LightType::POINT;
	sunLight.position = lightPos;
	sunLight.color = glm::vec3(1.0f);
	sunLight.ambient = glm::vec3(0.1f);
	sunLight.diffuse = glm::vec3(0.5f);
	sunLight.specular = glm::vec3(1.0f);
	sunLight.constant = 1.0f;
	sunLight.linear = 0.005f;
	sunLight.quadratic = 0.0025f;

	lightObject.setLight(sunLight);

	lightObjects.push_back(lightObject);
}

void Scene1::initEnvironment() {
	ResourceManager &rm = application->getResourceManager();

	auto sphereMesh = rm.getMesh("sphere", []() { return createSphereMesh(300, 300); });
	auto objectShader = rm.loadShader("earth", "shaders/vertex/uber.vs", "shaders/fragment/uber.fs");
	auto objectTexture = rm.loadTexture("earth", "assets/earth_day.jpg");

	Transform objectTransform{glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(1.25f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f};

	Object earthObject(sphereMesh, objectShader, objectTransform);

	earthObject.bodyTexture = objectTexture;
	earthObject.setMaterial(nullptr, nullptr, 16.0f);

	materialObjects.push_back(earthObject);
}

void Scene1::initSkybox() {
	ResourceManager &rm = application->getResourceManager();

	auto sphereMesh = rm.getMesh("sphere", []() { return createSphereMesh(300, 300); });
	auto skyShader = rm.loadShader("sky", "shaders/vertex/uber.vs", "shaders/fragment/uber.fs");
	auto skyTexture = rm.loadTexture("stars", "assets/stars.jpg");

	Transform skyTransform{glm::vec3(0.0f), glm::vec3(20.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f};

	Object skyObject(sphereMesh, skyShader, skyTransform);

	skyObject.isSkybox = true;
	skyObject.bodyTexture = skyTexture;

	skyObjects.push_back(skyObject);
}

void Scene1::update(float deltaTime) {
	updatePlanet(deltaTime);
	updateLights(deltaTime);
	updateCamera(deltaTime);
}

void Scene1::updatePlanet(float deltaTime) {
	static float theta = 0.0f;

	if (!materialObjects.empty()) {
		materialObjects[0].transform.position = glm::vec3(25.0f * cos(theta), 0.0f, 25.0f * sin(theta));
		materialObjects[0].transform.angle = theta;
	}

	theta += speed * deltaTime;
}

void Scene1::updateLights(float deltaTime) {
	static float theta = 0.0f;

	theta += speed * deltaTime;

	if (!lightObjects.empty()) {
		lightObjects[0].transform.angle = theta / 10.0f;
	}
}

void Scene1::updateCamera(float deltaTime) {
	if (!materialObjects.empty()) {
		camera.target = materialObjects[0].transform.position;
		camera.moveToward(deltaTime, speed);
	}
}

void Scene1::render(Camera &camera, float aspectRatio) {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (aspectRatio <= 0.0f) {
		aspectRatio = 0.1f;
	}

	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), aspectRatio, 0.1f, 500.0f);

	glm::mat4 view = camera.GetViewMatrix();

	std::vector<Light> activeLights;

	for (auto &lightObj : lightObjects) {
		if (lightObj.light.has_value()) {
			activeLights.push_back(lightObj.light.value());
		}
	}

	renderSkybox(projection, view, activeLights);
	renderEnvironment(projection, view, activeLights);
	renderLightMeshes(projection, view, activeLights);
}

void Scene1::renderSkybox(glm::mat4 &projection, glm::mat4 &view, std::vector<Light> &activeLights) {
	glm::mat4 skyboxView = glm::mat4(glm::mat3(view));

	glDepthMask(GL_FALSE);

	for (auto &skyObject : skyObjects) {
		skyObject.setProjection(projection);
		skyObject.setView(skyboxView);
		skyObject.setModel();
		skyObject.render(camera.pos, activeLights);
	}

	glDepthMask(GL_TRUE);
}

void Scene1::renderEnvironment(glm::mat4 &projection, glm::mat4 &view, std::vector<Light> &activeLights) {
	for (auto &materialObject : materialObjects) {
		materialObject.setProjection(projection);
		materialObject.setView(view);
		materialObject.setModel();
		materialObject.render(camera.pos, activeLights);
	}
}

void Scene1::renderLightMeshes(glm::mat4 &projection, glm::mat4 &view, std::vector<Light> &activeLights) {
	for (auto &lightObject : lightObjects) {
		lightObject.setProjection(projection);
		lightObject.setView(view);
		lightObject.setModel();
		lightObject.render(camera.pos, activeLights);
	}
}

void Scene1::processInput(Input &input, float deltaTime) {
	if (!input.getIsCameraActive()) {
		return;
	}

	if (input.isKeyPressed(GLFW_KEY_W))
		camera.processKeyboard(CameraMovement::FORWARD, deltaTime);

	if (input.isKeyPressed(GLFW_KEY_S))
		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);

	if (input.isKeyPressed(GLFW_KEY_A))
		camera.processKeyboard(CameraMovement::LEFT, deltaTime);

	if (input.isKeyPressed(GLFW_KEY_D))
		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);

	float xOffset = input.getMouseOffsetX();
	float yOffset = input.getMouseOffsetY();

	if (xOffset != 0.0f || yOffset != 0.0f) {
		camera.processMouseMovement(xOffset, yOffset);
	}

	float scrollOffset = input.getScrollOffsetY();

	if (scrollOffset != 0.0f) {
		camera.processMouseScroll(scrollOffset);
	}
}

Camera &Scene1::getCamera() { return camera; }

} // namespace DoNotOpenGL