#include "Torus.hpp"
#include "EngineClass.hpp"
#include "Camera.hpp"

void Engine::Engine::Initialize() {
	Entity::Torus::Initialize();
}

void Engine::Engine::Render(Camera& cam) {
	glBindVertexArray(u_VAO);
	Entity::Torus::Render(cam);
}