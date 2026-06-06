#pragma once

#include "scenes/scene.h"
#include "graphics/object.h"
#include <memory>

namespace DoNotOpenGL {

class Scene1 : public Scene {
  public:
	Scene1(Application *app);
	~Scene1() override = default;

	void init() override;
	void update(float deltaTime) override;
	void render(Camera &camera, float aspectRatio) override;

	void processInput(Input &input, float deltaTime) override;

	Camera &getCamera() override;

  private:
	Camera camera;
	std::vector<Object> objects;
};

} // namespace DoNotOpenGL