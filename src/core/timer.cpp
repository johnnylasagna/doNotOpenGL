#include "core/timer.h"

namespace DoNotOpenGL {

Timer::Timer() {
	previousTime = glfwGetTime();
	frameCount = 0;
}

void Timer::fps() {
	double currentTime = glfwGetTime();

	frameCount++;

	if (currentTime - previousTime >= 1.0) {

		std::cout << "fps: " << frameCount << std::endl;

		frameCount = 0;

		previousTime = currentTime;
	}
}

} // namespace DoNotOpenGL