#include "Torus.hpp"
#include "EngineClass.hpp"
#include "Camera.hpp"

void Engine::Engine::Initialize() {
	Torus::Initialize();
}

void Engine::Engine::Render(Camera& cam) {
	Torus::Render(cam);
}