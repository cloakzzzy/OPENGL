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

void Engine::Engine_::Initialize() {
	Window::Initialize();
}

void Engine::Engine_::Render(Camera& RenderCamera) {

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Entity::Entity_::GlobalUniforms_SSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(RenderCamera.position));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(glm::mat4), glm::value_ptr(RenderCamera.GetProjection()));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(RenderCamera.GetView()));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4), sizeof(unsigned int), &Entity::Lights::Num_DirectionalLights);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4) + sizeof(unsigned int), sizeof(unsigned int), &Entity::Lights::Num_PointLights);


	Entity::PointLight::UpdateBuffer();
	Entity::DirectionalLight::UpdateBuffer();


	glm::vec3 lightPos = RenderCamera.position;

	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 100.f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(1.0, 1.0, 1.0));
	lightSpaceMatrix = lightProjection * lightView;

	//Entity::Primitives::simpleDepthShader.Use();
	//Entity::Primitives::simpleDepthShader.SetMat4(Entity::Primitives::simpleDepthShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));

	glViewport(0, 0, 800, 600);
	glBindFramebuffer(GL_FRAMEBUFFER, Entity::Primitives::depthMapFBO);
	
	glClear(GL_DEPTH_BUFFER_BIT);
	


	Entity::Primitives::FloorShader.Use();
	Entity::Primitives::FloorShader.SetMat4(Entity::Primitives::uloc_view, glm::value_ptr(RenderCamera.GetView()));
	Entity::Primitives::FloorShader.SetMat4(Entity::Primitives::uloc_projection, glm::value_ptr(RenderCamera.GetProjection()));
	Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_cam_pos, RenderCamera.position.x, RenderCamera.position.y, RenderCamera.position.z);
	Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_col1, 0.1, 0.1, 0.1);
	Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_col2, 0.2, 0.2, 0.2);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	Entity::Primitives::Render<Entity::Torus>();
	Entity::Primitives::Render<Entity::Sphere>();
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	Entity::Primitives::debugDepthQuad.Use();
	Entity::Primitives::debugDepthQuad.SetFloat(Entity::Primitives::debugDepthQuad.GetUniformLocation("near_plane"), near_plane);
	Entity::Primitives::debugDepthQuad.SetFloat(Entity::Primitives::debugDepthQuad.GetUniformLocation("far_plane"), far_plane);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, Entity::Primitives::depthMap);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	float* data = new float[800 * 600];
	/*
	glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, data);

	// check min/max
	float minD = 1.0f, maxD = 0.0f;
	for (int i = 0; i < 800 * 600; ++i) {
		if (data[i] != 1.0f) {
			//std::cout << data[i] << '\n';
		}
	}
	delete[] data;
	*/

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

	
	
}

Engine::Window Engine::Engine_::CreateWindow(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, 
	float Red, float Green, float Blue) {
	return Window(WindowTitle, ScreenWidth, ScreenHeight, Red, Green, Blue);
}