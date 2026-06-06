#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace DoNotOpenGL {

struct Timer {
	double previousTime{};
    int frameCount{};

	Timer();

	void fps();
};

} // namespace DoNotOpenGL