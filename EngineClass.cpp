#include "Torus.hpp"
#include "EngineClass.hpp"

void Engine::Engine::Initialize() {
	Torus::Initialize(u_VAO);
}

void Engine::Engine::Render() {
	Torus::Render();
}