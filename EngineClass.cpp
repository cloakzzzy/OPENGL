#include "Torus.hpp"
#include "Sphere.hpp"
#include "EngineClass.hpp"
#include "Camera.hpp"
#include "Window.hpp"
#include "Primitives.hpp"
#include "PointLight.hpp"
#include "DirectionalLight.hpp"
#include "Lights.hpp"

unsigned int quadVAO = 0;
unsigned int quadVBO;
unsigned int passes = 0;

float near_plane = 0.05f, far_plane = 100.f;


//glm::vec3 lightDir(-0.707f);t

void Engine::Engine_::Initialize() {
	Window::Initialize();
}

void Engine::Engine_::Render(Entity::Camera& RenderCamera)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto directional_light : Entity::DirectionalLight::D_Objects) {
		directional_light->PerFrame(RenderCamera);


		Entity::PointLight::UpdateBuffer();
		Entity::DirectionalLight::UpdateBuffer();


		Entity::Primitives::FloorDepthShader.Use();
		glDrawArrays(GL_TRIANGLES, 0, 6);


		Entity::Primitives::Render_UsingDepthShader<Entity::Torus>();
		Entity::Primitives::Render_UsingDepthShader<Entity::Sphere>();


		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	Entity::Entity_::SetGlobalCameraData(RenderCamera.position, RenderCamera.GetProjection(), RenderCamera.GetView());

	Entity::PointLight::UpdateBuffer();
	Entity::DirectionalLight::UpdateBuffer();

	Entity::Primitives::FloorShader.Use();

	for (int i = 0; i < Entity::DirectionalLight::D_Objects.size(); i++) {
		GLint uloc_lsp = Entity::Primitives::FloorShader.GetUniformLocation("lightSpaceMatrix[" + std::to_string(i) + "]");
		Entity::Primitives::FloorShader.SetMat4(uloc_lsp, glm::value_ptr(Entity::DirectionalLight::D_Objects[i]->lightSpaceMatrix));

		GLint uloc_smpID = Entity::Primitives::FloorShader.GetUniformLocation("shadowMaps[" + std::to_string(i) + "]");
		Entity::Primitives::FloorShader.SetInt(uloc_smpID, i);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, Entity::DirectionalLight::DepthMaps[i]);
	}

	Entity::Primitives::Render<Entity::Torus>();

	Entity::Primitives::FloorShader.Use();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	Entity::Primitives::Render<Entity::Sphere>();

	glBindVertexArray(Window::vao);
	Window::vb.Bind();
	Window::ib.Bind();
	Window::s.Use();

	//glDrawElements(GL_TRIANGLES, Window::indices.size(), GL_UNSIGNED_INT, 0);

	//glEnable(GL_DEPTH_TEST);

	
}

Engine::Window Engine::Engine_::CreateWindow(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight,
	float Red, float Green, float Blue) {
	return Window(WindowTitle, ScreenWidth, ScreenHeight, Red, Green, Blue);
}