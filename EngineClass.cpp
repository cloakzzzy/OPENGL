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

	
	glBindVertexArray(Engine_::u_VAO);

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
	// END RENDER FROM LIGHT PERSPECTIVE

	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	{
		Entity::Primitives::debugDepthQuad.Use();
		Entity::Primitives::debugDepthQuad.SetFloat(Entity::Primitives::debugDepthQuad.GetUniformLocation("near_plane"), near_plane);
		Entity::Primitives::debugDepthQuad.SetFloat(Entity::Primitives::debugDepthQuad.GetUniformLocation("far_plane"), far_plane);
	}


	// render scene as normal 

	Entity::Entity_::SetGlobalCameraData(RenderCamera.position, RenderCamera.GetProjection(), RenderCamera.GetView());


	Entity::PointLight::UpdateBuffer();
	Entity::DirectionalLight::UpdateBuffer();

	Entity::Primitives::FloorShader.Use();

	
	for (int i = 0; i < Entity::DirectionalLight::D_Objects.size(); i++) {
		GLint uloc_lsp = Entity::Primitives::FloorShader.GetUniformLocation("lightSpaceMatrix[" + std::to_string(i) + "]");
		Entity::Primitives::FloorShader.SetMat4(uloc_lsp, glm::value_ptr(*(Entity::DirectionalLight::LightSpaceMatrices[i])));

		GLint uloc_smpID = Entity::Primitives::FloorShader.GetUniformLocation("shadowMaps[" + std::to_string(i) + "]");
		Entity::Primitives::FloorShader.SetInt(uloc_smpID, i);

		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, Entity::DirectionalLight::DepthMaps[i]);
	}

	
	/*
	Entity::Torus::PrimitiveShader.Use();
	Entity::Torus::PrimitiveShader.SetMat4(Entity::Torus::PrimitiveShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(dl.lightSpaceMatrix));

	Entity::Sphere::PrimitiveShader.Use();
	Entity::Sphere::PrimitiveShader.SetMat4(Entity::Sphere::PrimitiveShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(dl.lightSpaceMatrix));
*/

	

	/*
	Entity::Torus::PrimitiveShader.Use();
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, dl.DepthMap);

	Entity::Sphere::PrimitiveShader.Use();
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, dl.DepthMap);
	*/

	Entity::Primitives::debugDepthQuad.Use();

	//glActiveTexture(GL_TEXTURE6);
	//glBindTexture(GL_TEXTURE_2D, Entity::Primitives::depthMap);

	Entity::Primitives::Render<Entity::Torus>();

	Entity::Primitives::FloorShader.Use();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Start render from camera perspective
	
	Entity::Primitives::Render<Entity::Sphere>();

	//end render from camera perspective
	/*
	{
		Entity::Primitives::debugDepthQuad.Use();
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	*/
}

Engine::Window Engine::Engine_::CreateWindow(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, 
	float Red, float Green, float Blue) {
	return Window(WindowTitle, ScreenWidth, ScreenHeight, Red, Green, Blue);
}