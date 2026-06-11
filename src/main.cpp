// extern "C" {
//     __declspec(dllexport) unsigned long NvOptimusEnablement = 1;
// }

#include "core/editor.h"
#include "core/application.h"
#include "scenes/scene.h"
#include "scenes/scene_1.h"
// #include "scenes/scene_2.h"
// #include "scenes/scene_3.h"
// #include "scenes/scene_4.h"

#include <memory>

int main() {
	DoNotOpenGL::Editor editor;

	DoNotOpenGL::Application app(editor);

	app.editorMode = DoNotOpenGL::EditorMode::Video;

	if (!app.init()) {
		return -1;
	}

	auto scene1 = std::make_unique<DoNotOpenGL::Scene1>(&app);
	editor.addScene(std::move(scene1));

	// auto scene2 = std::make_unique<DoNotOpenGL::Scene2>(&app);
	// editor.addScene(std::move(scene2));

	// auto scene3 = std::make_unique<DoNotOpenGL::Scene3>(&app);
	// editor.addScene(std::move(scene3));

	// auto scene4 = std::make_unique<DoNotOpenGL::Scene4>(&app);
	// editor.addScene(std::move(scene4));

	// editor.addClip(-1, 0);
	editor.addClip(0, 35);
	// editor.addClip(2, 3);
	// editor.addClip(3, 3.5);
	// editor.addClip(0, 6.5);
	// editor.addClip(2, 3.5);
	// editor.addClip(3, 3.5);
	// editor.addClip(0, 3.5);
	// editor.addClip(2, 3.5);
	// editor.addClip(1, 3);
	// editor.addClip(3, 3.25);
	// editor.addClip(2, 3.5);
	// editor.addClip(1, 3.25);
	// editor.addClip(0, 3);

	editor.resetDurations();

	app.run();

	return 0;
}