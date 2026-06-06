#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "settings.h"

namespace DoNotOpenGL {

GLFWwindow *createWindow();

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

} // namespace DoNotOpenGL