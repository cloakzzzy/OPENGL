#include "Torus.hpp"
#include "Sphere.hpp"
#include "EngineClass.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "Primitives.hpp"
#include "PointLight.hpp"
#include "DirectionalLight.hpp"
#include "Lights.hpp"

void Engine::Engine_::Initialize() {
	Window::Initialize();
}

void Engine::Engine_::Render(Camera& RenderCamera) {

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Entity::Entity_::GlobalUniforms_SSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(RenderCamera.position));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(glm::mat4), glm::value_ptr(RenderCamera.GetProjection()));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(RenderCamera.GetView()));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4), sizeof(unsigned int), &Entity::Lights::Num_DirectionalLights);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4) + sizeof(unsigned int), sizeof(unsigned int), &Entity::Lights::Num_PointLights);


	Entity::PointLight::UpdateBuffer();
	Entity::DirectionalLight::UpdateBuffer();

	
	Entity::Primitives::Render<Entity::Torus>();
	Entity::Primitives::Render<Entity::Sphere>();
	
	Entity::Primitives::RenderFloor(RenderCamera);

}

Engine::Window Engine::Engine_::CreateWindow(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, 
	float Red, float Green, float Blue) {
	return Window(WindowTitle, ScreenWidth, ScreenHeight, Red, Green, Blue);
}