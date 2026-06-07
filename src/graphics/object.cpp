#include "graphics/object.h"

namespace DoNotOpenGL {

Object::Object(std::unique_ptr<Mesh> mesh, Shader *shader, std::vector<Texture *> textures)
    : mesh(std::move(mesh)), shader(shader), textures(textures) {
	shader->use();
	for (int i = 0; i < textures.size(); i++) {
		std::string name = "texture";
		name.push_back('0' + i);
		shader->setInt(name, i);
	}
}

void Object::setMaterialProperties(Texture *diffuse, Texture *specular, float shininess) {
	shader->use();
	shader->setInt("material.diffuse", (int)textures.size());
	shader->setInt("material.specular", (int)textures.size() + 1);
	shader->setFloat("material.shininess", shininess);

	diffuseTexture = diffuse;
	specularTexture = specular;
}

void Object::setPosition(float x, float y, float z) { position = glm::vec3(x, y, z); }

glm::vec3 &Object::getPosition() { return position; }

void Object::setScale(float x, float y, float z) { scale = glm::vec3(x, y, z); }

void Object::setRotation(float theta, float x, float y, float z) {
	angle = theta;
	rotation = glm::vec3(x, y, z);
}

void Object::render() {
	shader->use();
	for (int i = 0; i < textures.size(); i++) {
		textures[i]->use(i);
	}
	if (diffuseTexture)
		diffuseTexture->use((int)textures.size());
	if (specularTexture)
		specularTexture->use((int)textures.size() + 1);
	glBindVertexArray(mesh->VAO);

	glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
}

void Object::setProjection(glm::mat4 &projection) {
	shader->use();
	shader->setMat4("projection", projection);
}

void Object::setView(glm::mat4 &view) {
	shader->use();
	shader->setMat4("view", view);
};

void Object::setViewPosition(glm::vec3 &viewPosition) {
	shader->use();
	shader->setVec3("viewPos", viewPosition);
}

void Object::setModel() {
	shader->use();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::rotate(model, angle, rotation);
	model = glm::scale(model, scale);
	shader->setMat4("model", model);

	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
	shader->setMat3("normalMatrix", normalMatrix);
}

Shader *Object::getShader() { return shader; }

} // namespace DoNotOpenGL