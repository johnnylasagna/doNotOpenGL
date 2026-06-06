#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

#include "core/errors.h"

namespace DoNotOpenGL {

struct Shader {
	unsigned int id{};
	std::unordered_map<std::string, int> uniformLocations;

	[[nodiscard]]
	Shader(const char *vertexPath, const char *fragmentPath);
	~Shader();

	Shader(const Shader &) = delete;
	Shader &operator=(const Shader &) = delete;

	Shader(Shader &&other) noexcept;
	Shader &operator=(Shader &&other) noexcept;

	[[nodiscard]]
	unsigned int createShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource);

	void use() const;

	int getUniformLocation(const std::string &name);

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w) const;
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;
};

} // namespace DoNotOpenGL