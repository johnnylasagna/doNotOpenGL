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
	Object(std::shared_ptr<Mesh> mesh, Shader *shader, std::vector<Texture *> texture);
	void setPosition(float x, float y, float z);
	void setPosition(glm::vec3 &objectPosition);
	void setScale(float x, float y, float z);
	void setScale(glm::vec3 &objectScale);
	void setRotation(float theta, float x, float y, float z);
	void setRotation(float theta, glm::vec3 &objectRotation);
	void setProjection(glm::mat4 &projection);
	void setView(glm::mat4 &view);
	void setModel();

	Shader *getShader();
	glm::vec3 &getPosition();

	virtual void render() = 0;

  protected:
	std::shared_ptr<Mesh> mesh;
	Shader *shader;
	std::vector<Texture *> textures;

	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	float angle;
};

class LightObject : public Object {
  public:
	using Object::Object;
	void setProperties(const char *type, glm::vec3 &color, glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular);
	glm::vec3 &getAmbient();
	glm::vec3 &getDiffuse();
	glm::vec3 &getSpecular();

	void render() override;

  private:
	std::string type;
	glm::vec3 color;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class MaterialObject : public Object {
  public:
	using Object::Object;
	void setViewPosition(glm::vec3 &viewPosition);
	void setMaterialProperties(Texture *diffuse, Texture *specular, float shininess);
	void setLightProperties(int id, LightObject &lightObject);

	void render() override;

  private:
	Texture *diffuseTexture = nullptr;
	Texture *specularTexture = nullptr;
	float shininess;
};

} // namespace DoNotOpenGL