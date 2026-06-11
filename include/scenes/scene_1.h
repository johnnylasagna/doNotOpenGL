#pragma once

#include "scenes/scene.h"
#include "graphics/object.h"
#include <memory>
#include <vector>

namespace DoNotOpenGL {

class Scene1 : public Scene {
  public:
	Scene1(Application *app);
	~Scene1() override = default;

	void init() override;
	void reset() override;
	void update(float deltaTime) override;
	void render(Camera &camera, float aspectRatio) override;

	void processInput(Input &input, float deltaTime) override;

	Camera &getCamera() override;

  private:
	void initCamera();
	void initLights();
	void initEnvironment();
	void initSkybox();

	void updatePlanet(float deltaTime);
	void updateLights(float deltaTime);
	void updateCamera(float deltaTime);

	void renderSkybox(glm::mat4 &projection, glm::mat4 &view, std::vector<Light> &activeLights);

	void renderEnvironment(glm::mat4 &projection, glm::mat4 &view, std::vector<Light> &activeLights);

	void renderLightMeshes(glm::mat4 &projection, glm::mat4 &view, std::vector<Light> &activeLights);
};

} // namespace DoNotOpenGL