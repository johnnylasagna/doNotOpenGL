#include "core/errors.h"

namespace DoNotOpenGL {

[[nodiscard]]
bool checkShaderCompilation(unsigned int shader, const char *shaderType) {
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::" << shaderType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	return true;
}

[[nodiscard]]
bool checkProgramLinking(unsigned int program, const char *programType) {
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::" << programType << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	return true;
}

} // namespace DoNotOpenGL