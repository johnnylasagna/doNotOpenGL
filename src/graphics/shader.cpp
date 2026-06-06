#include "graphics/shader.h"

namespace DoNotOpenGL {

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vertexShaderFile;
	std::ifstream fragmentShaderFile;

	vertexShaderFile.open(vertexPath);
	fragmentShaderFile.open(fragmentPath);

	if (!vertexShaderFile.is_open() || !fragmentShaderFile.is_open()) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	} else {
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}

	const char *vertexShaderCode = vertexCode.c_str();
	const char *fragmentShaderCode = fragmentCode.c_str();

	id = createShaderProgram(vertexShaderCode, fragmentShaderCode);
}

Shader::~Shader() {
	glDeleteProgram(id);
}

Shader::Shader(Shader &&other) noexcept : id(other.id), uniformLocations(std::move(other.uniformLocations)) {
	other.id = 0;
}

Shader &Shader::operator=(Shader &&other) noexcept {
	if (this != &other) {
		glDeleteProgram(id); 

		id = other.id;
		uniformLocations = std::move(other.uniformLocations);

		other.id = 0;
	}
	return *this;
}

[[nodiscard]]
unsigned int Shader::createShaderProgram(const char *vertexShaderSource, const char *fragmentShaderSource) {
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	if (!checkShaderCompilation(vertexShader, "VERTEX")) {
		return 0;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	if (!checkShaderCompilation(fragmentShader, "FRAGMENT")) {
		glDeleteShader(vertexShader);
		return 0;
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	if (!checkProgramLinking(shaderProgram, "SHADER")) {
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return 0;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void Shader::use() const {
	glUseProgram(id);
}

int Shader::getUniformLocation(const std::string &name) {

	if (uniformLocations.contains(name)) {
		return uniformLocations[name];
	}

	int location = glGetUniformLocation(id, name.c_str());

	uniformLocations[name] = location;

	return location;
}

void Shader::setBool(const std::string &name, bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const {
	glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const std::string &name, float x, float y) const {
	glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const {
	glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const {
	glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const {
	glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

} // namespace DoNotOpenGL