#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "graphics/mesh.h"
#include "graphics/shader.h"
#include "graphics/texture.h"

namespace DoNotOpenGL {

class Object {
  public:
	Object(std::unique_ptr<Mesh> mesh, Shader *shader, std::vector<Texture *> texture);
	void setMaterialProperties(Texture *diffuse, Texture *specular, float shininess);
	void setPosition(float x, float y, float z);
	glm::vec3 &getPosition();
	void setScale(float x, float y, float z);
	void setRotation(float theta, float x, float y, float z);
	void setProjection(glm::mat4 &projection);
	void setView(glm::mat4 &view);
	void setViewPosition(glm::vec3 &viewPosition);
	void setModel();
	Shader *getShader();

	void render();

  private:
	std::unique_ptr<Mesh> mesh;
	Shader *shader;
	std::vector<Texture *> textures;
	Texture* diffuseTexture = nullptr;
	Texture* specularTexture = nullptr;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	float angle;
};

} // namespace DoNotOpenGL