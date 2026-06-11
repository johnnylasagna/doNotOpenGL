#include "core/resourceManager.h"
#include <iostream>

namespace DoNotOpenGL {

std::shared_ptr<Shader> ResourceManager::loadShader(const std::string &name, const char *vShaderFile, const char *fShaderFile) {
    if (shaders.find(name) == shaders.end()) {
        shaders[name] = std::make_shared<Shader>(vShaderFile, fShaderFile);
    }
    return shaders[name];
}

std::shared_ptr<Shader> ResourceManager::getShader(const std::string &name) {
    if (shaders.find(name) != shaders.end()) {
        return shaders[name];  // remove .get()
    }
    std::cerr << "Warning: Shader '" << name << "' not found!" << std::endl;
    return nullptr;
}

std::shared_ptr<Texture> ResourceManager::loadTexture(const std::string &name, const char *file) {
    if (textures.find(name) == textures.end()) {
        textures[name] = std::make_shared<Texture>(file);
    }
    return textures[name];
}

std::shared_ptr<Texture> ResourceManager::getTexture(const std::string &name) {
    if (textures.find(name) != textures.end()) {
        return textures[name];  // remove .get()
    }
    std::cerr << "Warning: Texture '" << name << "' not found!" << std::endl;
    return nullptr;
}

std::shared_ptr<Mesh> ResourceManager::getMesh(const std::string &name, std::function<std::shared_ptr<Mesh>()> creator) {
    if (meshes.count(name)) {
        return meshes[name];
    }
    meshes[name] = creator();
    return meshes[name];
}

void ResourceManager::clear() {
    shaders.clear();
    textures.clear();
    meshes.clear(); 
}

} // namespace DoNotOpenGL