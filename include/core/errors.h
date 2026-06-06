#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace DoNotOpenGL {

[[nodiscard]]
bool checkShaderCompilation(unsigned int vertexShader, const char *shaderType);

[[nodiscard]]
bool checkProgramLinking(unsigned int program, const char *programType);

} // namespace DoNotOpenGL
