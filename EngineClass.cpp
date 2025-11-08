#include "Torus.hpp"
#include "EngineClass.hpp"
#include "Camera.hpp"


void Engine::Engine::Initialize() {
	Window::Initialize();
}

void Engine::Engine::Render(Camera& cam) {
	glBindVertexArray(u_VAO);
	Entity::Torus::Render(cam);
}

Engine::Window Engine::Engine::CreateWindow(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, 
	float Red, float Green, float Blue) {
	return Window(WindowTitle, ScreenWidth, ScreenHeight, Red, Green, Blue);
}