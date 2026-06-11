#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <functional>

#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/mesh.h"

namespace DoNotOpenGL {

class ResourceManager {
  public:
	// We delete copy constructors to prevent accidental duplication
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager &) = delete;
	ResourceManager &operator=(const ResourceManager &) = delete;

	// Shaders
	std::shared_ptr<Shader> loadShader(const std::string &name, const char *vShaderFile, const char *fShaderFile);
	std::shared_ptr<Shader> getShader(const std::string &name);
	std::shared_ptr<Texture> loadTexture(const std::string &name, const char *file);
	std::shared_ptr<Texture> getTexture(const std::string &name);
	std::shared_ptr<Mesh> getMesh(const std::string &name, std::function<std::shared_ptr<Mesh>()> creator);

	// Clears all loaded resources
	void clear();

  private:
	std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshes;
};

} // namespace DoNotOpenGL