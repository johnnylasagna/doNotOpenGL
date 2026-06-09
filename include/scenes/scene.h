#pragma once

#include "graphics/camera.h"
#include "core/input.h"

namespace DoNotOpenGL {

class Application;

class Scene {
  public:
	Scene(Application *app) : application(app) {}
	virtual ~Scene() = default;

	virtual void init() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void render(Camera &camera, float aspectRatio) = 0;
	virtual void renderUI() {};
	virtual void processInput(Input &input, float deltaTime) = 0;
	virtual Camera &getCamera() = 0;

  protected:
	Application *application;
};

} // namespace DoNotOpenGL