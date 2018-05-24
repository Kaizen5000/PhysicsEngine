#include "PhysicsEngineApp.h"

int main() {
	
	// allocation
	auto app = new PhysicsEngineApp();

	// initialise and loop
	app->run("Physics Engine", 1280, 720, false);

	// deallocation
	delete app;

	return 0;

}