#pragma once

#include "graphics/camera.h"
#include "core/input.h"
#include "graphics/object.h"

namespace DoNotOpenGL {

class Application;

class Scene {
  public:
	Scene(Application *app) : application(app) {}
	virtual ~Scene() = default;

	virtual void init() = 0;
	virtual void reset() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render(Camera &camera, float aspectRatio) = 0;
	virtual void renderUI();
	virtual void processInput(Input &input, float deltaTime) = 0;
	virtual Camera &getCamera() = 0;

	float timeElapsed = 0;
	float speed = 0;

  protected:
	Application *application;
	Camera camera;

	void renderLightUI(Light &light, int index);
	void renderCameraUI(Camera &camera);
	void renderObjectUI(Object &object, const std::string& category, int index);
	void renderTransformUI(Transform &transform, const std::string &prefix);
	void renderMaterialUI(Material &material, const std::string &prefix);

	std::vector<Object> lightObjects;
	std::vector<Object> materialObjects;
	std::vector<Object> skyObjects;
	std::vector<Object> rayObjects;
};

} // namespace DoNotOpenGL