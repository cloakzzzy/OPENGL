#include "Torus.hpp"
#include "Sphere.hpp"
#include "EngineClass.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "Primitives.hpp"
#include "PointLight.hpp"

void Engine::Engine::Initialize() {
	Window::Initialize();
}

void Engine::Engine::Render(Camera& cam) {
	Entity::PointLight::UpdateBuffer();
	glBindVertexArray(u_VAO);
	Entity::Torus::Render(cam);
	Entity::Sphere::Render(cam);
	Entity::Primitives::RenderFloor(cam);
	
}

Engine::Window Engine::Engine::CreateWindow(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, 
	float Red, float Green, float Blue) {
	return Window(WindowTitle, ScreenWidth, ScreenHeight, Red, Green, Blue);
}