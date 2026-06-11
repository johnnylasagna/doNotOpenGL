#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> // Added: Necessary for glm::value_ptr
#include <glm/gtx/quaternion.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <numeric>
#include <string>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "scenes/scene.h"

namespace DoNotOpenGL {

void Scene::renderUI() {
	ImGui::Begin("Camera");
	renderCameraUI(camera);
	ImGui::End();

	ImGui::Begin("Lights");
	for (int i = 0; i < lightObjects.size(); i++) {
		if (lightObjects[i].light.has_value()) {
			renderLightUI(lightObjects[i].light.value(), i);
			renderObjectUI(lightObjects[i], "Light", i);
		}
	}
	ImGui::End();

	ImGui::Begin("Environment");
	for (int i = 0; i < materialObjects.size(); i++) {
		renderObjectUI(materialObjects[i], "Env", i);
	}
	ImGui::End();

	ImGui::Begin("Light Rays");
	for (int i = 0; i < rayObjects.size(); i++) {
		renderObjectUI(rayObjects[i], "Ray", i);
	}
	ImGui::End();

	ImGui::Begin("Skybox");
	for (int i = 0; i < skyObjects.size(); i++) {
		renderObjectUI(skyObjects[i], "Sky", i);
	}
	ImGui::End();

	ImGui::Begin("Time");
	ImGui::SliderFloat("Time elapsed", &timeElapsed, 0.0f, 100.0f);
	ImGui::SliderFloat("Speed", &speed, 0.0f, 5.0f);
	ImGui::End();
}

void Scene::renderLightUI(Light &light, int index) {
	std::string label = "Light " + std::to_string(index);

	if (ImGui::CollapsingHeader(label.c_str())) {
		std::string prefix = label + " ";
		ImGui::SliderFloat3((prefix + "Position").c_str(), glm::value_ptr(light.position), -500.0f, 500.0f);
		ImGui::SliderFloat3((prefix + "Direction").c_str(), glm::value_ptr(light.direction), -1.0f, 1.0f);
		ImGui::ColorEdit3((prefix + "Color").c_str(), glm::value_ptr(light.color));
		ImGui::ColorEdit3((prefix + "Ambient").c_str(), glm::value_ptr(light.ambient));
		ImGui::ColorEdit3((prefix + "Diffuse").c_str(), glm::value_ptr(light.diffuse));
		ImGui::ColorEdit3((prefix + "Specular").c_str(), glm::value_ptr(light.specular));
		ImGui::SliderFloat((prefix + "Intensity").c_str(), &light.intensity, 0.0f, 100.0f);
		ImGui::Separator();
		ImGui::Text("Attenuation");
		ImGui::SliderFloat((prefix + "Constant").c_str(), &light.constant, 0.0f, 5.0f);
		ImGui::SliderFloat((prefix + "Linear").c_str(), &light.linear, 0.0f, 1.0f);
		ImGui::SliderFloat((prefix + "Quadratic").c_str(), &light.quadratic, 0.0f, 1.0f);
		ImGui::Separator();
		ImGui::Text("Spotlight");
		ImGui::SliderFloat((prefix + "Inner Cutoff").c_str(), &light.innerCutoff, 0.0f, 1.0f);
		ImGui::SliderFloat((prefix + "Outer Cutoff").c_str(), &light.outerCutoff, 0.0f, 1.0f);

		int currentType = static_cast<int>(light.type);
		if (ImGui::Combo((prefix + "Type").c_str(), &currentType, "Directional\0Point\0Spot\0\0")) {
			light.type = static_cast<decltype(light.type)>(currentType);
		}
	}
}

void Scene::renderCameraUI(Camera &camera) {
	ImGui::SliderFloat3("Position", glm::value_ptr(camera.pos), -500.0f, 500.0f);
	ImGui::SliderFloat3("Front", glm::value_ptr(camera.front), -1.0f, 1.0f);
	ImGui::SliderFloat3("Up", glm::value_ptr(camera.up), -1.0f, 1.0f);
	ImGui::SliderFloat("Yaw", &camera.yaw, -360.0f, 360.0f);
	ImGui::SliderFloat("Pitch", &camera.pitch, -89.0f, 89.0f);
	ImGui::SliderFloat("Zoom", &camera.zoom, 1.0f, 120.0f);
	ImGui::SliderFloat("Movement Speed", &camera.speed, 0.1f, 100.0f);
	ImGui::SliderFloat("Mouse Sensitivity", &camera.mouseSensitivity, 0.001f, 1.0f);

	int currentMode = static_cast<int>(camera.mode);
	if (ImGui::Combo("Mode", &currentMode, "Fly\0Creative\0\0")) {
		camera.mode = static_cast<decltype(camera.mode)>(currentMode);
	}

	ImGui::Checkbox("Target Set", (bool *)&camera.targetSet);
}

void Scene::renderObjectUI(Object &object, const std::string &category, int index) {
	// Uses the category parameter to avoid identical "Object 0" title collisions across windows
	std::string label = category + " Object " + std::to_string(index);

	if (ImGui::CollapsingHeader(label.c_str())) {
		renderTransformUI(object.transform, label);
		ImGui::Separator();
		ImGui::Checkbox((label + " Is Light Source").c_str(), &object.isLightSource);
		ImGui::Checkbox((label + " Is Skybox").c_str(), &object.isSkybox);
		ImGui::ColorEdit3((label + " Source Light Color").c_str(), glm::value_ptr(object.sourceLightColor));
		if (object.material.has_value()) {
			renderMaterialUI(object.material.value(), label);
		}
	}
}

void Scene::renderTransformUI(Transform &transform, const std::string &prefix) {
	ImGui::Text("Transform");
	ImGui::SliderFloat3((prefix + " Position").c_str(), glm::value_ptr(transform.position), -500.0f, 500.0f);
	ImGui::SliderFloat3((prefix + " Rotation Axis").c_str(), glm::value_ptr(transform.rotation), -1.0f, 1.0f);
	ImGui::SliderFloat((prefix + " Rotation Angle").c_str(), &transform.angle, -360.0f, 360.0f);
	ImGui::SliderFloat3((prefix + " Scale").c_str(), glm::value_ptr(transform.scale), 0.01f, 100.0f);
}

void Scene::renderMaterialUI(Material &material, const std::string &prefix) {
	ImGui::Text("Material");
	ImGui::SliderFloat((prefix + " Shininess").c_str(), &material.shininess, 1.0f, 512.0f);

	bool hasDiffuse = material.diffuseTexture != nullptr;
	bool hasSpecular = material.specularTexture != nullptr;

	// Visualizing state cleanly as a non-interactive, read-only checkbox status layout
	ImGui::BeginDisabled();
	ImGui::Checkbox((prefix + " Has Diffuse").c_str(), &hasDiffuse);
	ImGui::Checkbox((prefix + " Has Specular").c_str(), &hasSpecular);
	ImGui::EndDisabled();
}

} // namespace DoNotOpenGL