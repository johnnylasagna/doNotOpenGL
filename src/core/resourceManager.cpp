#include "core/resourceManager.h"
#include <iostream>

namespace DoNotOpenGL {

Shader* ResourceManager::loadShader(const std::string& name, const char* vShaderFile, const char* fShaderFile) {
    if (shaders.find(name) == shaders.end()) {
        shaders[name] = std::make_unique<Shader>(vShaderFile, fShaderFile);
    }
    return shaders[name].get();
}

Shader* ResourceManager::getShader(const std::string& name) {
    if (shaders.find(name) != shaders.end()) {
        return shaders[name].get();
    }
    std::cerr << "Warning: Shader '" << name << "' not found!" << std::endl;
    return nullptr;
}

Texture* ResourceManager::loadTexture(const std::string& name, const char* file) {
    if (textures.find(name) == textures.end()) {
        textures[name] = std::make_unique<Texture>(file);
    }
    return textures[name].get();
}

Texture* ResourceManager::getTexture(const std::string& name) {
    if (textures.find(name) != textures.end()) {
        return textures[name].get();
    }
    std::cerr << "Warning: Texture '" << name << "' not found!" << std::endl;
    return nullptr;
}

void ResourceManager::clear() {
    shaders.clear();
    textures.clear();
}

} // namespace DoNotOpenGL