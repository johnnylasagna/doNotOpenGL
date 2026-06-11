#include "graphics/object.h"

namespace DoNotOpenGL {

Object::Object(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, Transform transform)
    : mesh(mesh), shader(shader), transform(transform) {}

void Object::setProjection(const glm::mat4 &projection) {
	shader->use();
	shader->setMat4("projection", projection);
}

void Object::setView(const glm::mat4 &view) {
	shader->use();
	shader->setMat4("view", view);
};

void Object::setModel() {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, transform.position);
	model = glm::rotate(model, transform.angle, transform.rotation);
	model = glm::scale(model, transform.scale);
	shader->setMat4("model", model);

	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
	shader->setMat3("normalMatrix", normalMatrix);
}

void Object::render(glm::vec3 &viewPos, std::vector<Light>& lights) {
	shader->use();

	if (bodyTexture) {
		bodyTexture->use(0);
		shader->setInt("bodyTexture", 0);
		shader->setBool("hasBodyTexture", true);
	} else {
		shader->setBool("hasBodyTexture", false);
	}

	if (heightMap) {
		heightMap->use(1);
		shader->setInt("heightMap", 1);
		shader->setBool("hasHeightMap", true);
	} else {
		shader->setBool("hasHeightMap", false);
	}

	shader->setVec3("viewPos", viewPos);

	shader->setBool("isLightSource", isLightSource);
	shader->setBool("isSkybox", isSkybox);
	shader->setBool("hasMaterial", material.has_value());

	if (isLightSource) {
		shader->setVec3("lightColor", sourceLightColor);
	}

	if (!isSkybox && !isLightSource) {
		shader->setInt("lightCount", lights.size());
		for (int i = 0; i < lights.size(); i++) {
			Light &light = lights[i];
			std::string prefix = "lights[" + std::to_string(i) + "].";
			setLightSource(prefix, light);
		}
	}

	if (material) {
		auto &mat = material.value();

		if (mat.diffuseTexture) {
			mat.diffuseTexture->use(2);
			shader->setInt("material.diffuse", 2);
			shader->setBool("material.hasDiffuseMap", true);
		} else {
			shader->setBool("material.hasDiffuseMap", false);
		}

		if (mat.specularTexture) {
			mat.specularTexture->use(3);
			shader->setInt("material.specular", 3);
			shader->setBool("material.hasSpecularMap", true);
		} else {
			shader->setBool("material.hasSpecularMap", false);
		}

		shader->setFloat("material.shininess", mat.shininess);
	}

	glBindVertexArray(mesh->VAO);

	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
}

void Object::setMaterial(const std::shared_ptr<Texture>& diffuse, const std::shared_ptr<Texture>& specular, float shininess) {

	material = Material{diffuse, specular, shininess};

	shader->use();
	shader->setInt("material.diffuse", 2);
	shader->setInt("material.specular", 3);
	shader->setFloat("material.shininess", shininess);
}

void Object::setLight(Light light) { this->light = light; }

void Object::setLightSource(const std::string& prefix, const Light &light) {
	shader->setInt(prefix + "type", static_cast<int>(light.type));
	shader->setVec3(prefix + "position", light.position);
	shader->setVec3(prefix + "direction", light.direction);
	shader->setVec3(prefix + "color", light.color);
	shader->setVec3(prefix + "ambient", light.ambient);
	shader->setVec3(prefix + "diffuse", light.diffuse);
	shader->setVec3(prefix + "specular", light.specular);
	shader->setFloat(prefix + "intensity", light.intensity);
	shader->setFloat(prefix + "constant", light.constant);
	shader->setFloat(prefix + "linear", light.linear);
	shader->setFloat(prefix + "quadratic", light.quadratic);
	shader->setFloat(prefix + "innerCutoff", light.innerCutoff);
	shader->setFloat(prefix + "outerCutoff", light.outerCutoff);
}

} // namespace DoNotOpenGL