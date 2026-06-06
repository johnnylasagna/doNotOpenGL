#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "graphics/shader.h"
#include "graphics/texture.h"

namespace DoNotOpenGL {

class ResourceManager {
public:
    // We delete copy constructors to prevent accidental duplication
    ResourceManager() = default;
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Shaders
    Shader* loadShader(const std::string& name, const char* vShaderFile, const char* fShaderFile);
    Shader* getShader(const std::string& name);

    // Textures
    Texture* loadTexture(const std::string& name, const char* file);
    Texture* getTexture(const std::string& name);

    // Clears all loaded resources
    void clear();

private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
};

} // namespace DoNotOpenGL