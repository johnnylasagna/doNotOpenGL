extern "C" {
    __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
}

#include "core/application.h"
#include "scenes/scene_1.h"

int main() {
	DoNotOpenGL::Application app;

	if (!app.init()) {
        return -1;
    }
	
	app.setScene(std::make_unique<DoNotOpenGL::Scene1>(&app));

	app.run();

	return 0;
}