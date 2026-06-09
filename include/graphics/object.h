#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <optional>

#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"

namespace DoNotOpenGL {

struct Transform {
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0f);
	float angle = 0.0f;
};

struct Material {
	Texture *diffuseTexture = nullptr;
	Texture *specularTexture = nullptr;
	float shininess = 0.0f;
};

enum class LightType { DIRECTIONAL, POINT, SPOT };

struct Light {
	LightType type = LightType::POINT;

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);

	glm::vec3 color = glm::vec3(1.0f);

	glm::vec3 ambient = glm::vec3(0.1f);
	glm::vec3 diffuse = glm::vec3(1.0f);
	glm::vec3 specular = glm::vec3(1.0f);

	// point source
	float intensity = 1.0f;
	float constant = 1.0f;
	float linear = 1.0f;
	float quadratic = 1.0f;

	// spotlight source
	float innerCutoff = 0.0f;
	float outerCutoff = 0.0f;
};

struct Object {
	std::shared_ptr<Mesh> mesh;
	Shader *shader;
	Texture *bodyTexture = nullptr;
	Texture *heightMap = nullptr;

	Transform transform;
	std::optional<Material> material;
	std::optional<Light> light;

	bool isLightSource = false;
	bool isSkybox = false;
	glm::vec3 sourceLightColor = glm::vec3(1.0f);

	Object(std::shared_ptr<Mesh> mesh, Shader *shader, Transform transform);

	void render(glm::vec3 &viewPos, std::vector<Light> lights);

	void setProjection(glm::mat4 &projection);
	void setView(glm::mat4 &view);
	void setModel();

	void setMaterial(Texture *diffuse, Texture *specular, float shininess);
	void setLight(Light light);

	void setLightSource(std::string id, Light &light);
};

} // namespace DoNotOpenGL