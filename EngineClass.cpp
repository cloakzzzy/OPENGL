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

float orthosize = 60.0f;

/*
glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

glBindVertexArray(Engine_::u_VAO);

glm::mat4 view = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

float near_plane = 0.2f, far_plane = 100.f;


glBindBuffer(GL_SHADER_STORAGE_BUFFER, Entity::Entity_::GlobalUniforms_SSBO);
glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(lightPos));
glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(glm::mat4), glm::value_ptr(glm::ortho(-orthosize, orthosize, -orthosize, orthosize, near_plane, far_plane)));
glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4), sizeof(unsigned int), &Entity::Lights::Num_DirectionalLights);
glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4) + sizeof(unsigned int), sizeof(unsigned int), &Entity::Lights::Num_PointLights);


Entity::PointLight::UpdateBuffer();
Entity::DirectionalLight::UpdateBuffer();


glm::mat4 lightProjection, lightView;
glm::mat4 lightSpaceMatrix;
lightProjection = glm::ortho(-orthosize, orthosize, -orthosize, orthosize, near_plane, far_plane);
lightView = view;
lightSpaceMatrix = lightProjection * lightView;

Entity::Primitives::FloorDepthShader.Use();
Entity::Primitives::FloorDepthShader.SetMat4(Entity::Primitives::FloorDepthShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));

Entity::Torus::DepthShader.Use();
Entity::Torus::DepthShader.SetMat4(Entity::Torus::DepthShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));

Entity::Sphere::DepthShader.Use();
Entity::Sphere::DepthShader.SetMat4(Entity::Sphere::DepthShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));


glViewport(0, 0, 4096, 4096);
glBindFramebuffer(GL_FRAMEBUFFER, Entity::Primitives::depthMapFBO);


glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//Entity::Primitives::FloorDepthShader.Use();
//Entity::Primitives::FloorDepthShader.SetMat4(Entity::Primitives::FloorDepthShader.GetUniformLocation("view"), glm::value_ptr(view));
//Entity::Primitives::FloorDepthShader.SetMat4(Entity::Primitives::FloorDepthShader.GetUniformLocation("projection"), glm::value_ptr(RenderCamera.GetProjection()));
//Entity::Primitives::FloorDepthShader.SetVec3(Entity::Primitives::FloorDepthShader.GetUniformLocation("cam_pos"), lightPos.x, lightPos.y, lightPos.z);
//Entity::Primitives::FloorDepthShader.SetVec3(Entity::Primitives::uloc_col1, 0.1, 0.1, 0.1);
//Entity::Primitives::FloorDepthShader.SetVec3(Entity::Primitives::uloc_col2, 0.2, 0.2, 0.2);

/*
Entity::Primitives::FloorShader.Use();
Entity::Primitives::FloorShader.SetMat4(Entity::Primitives::uloc_view, glm::value_ptr(view));
Entity::Primitives::FloorShader.SetMat4(Entity::Primitives::uloc_projection, glm::value_ptr(RenderCamera.GetProjection()));
Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_cam_pos, lightPos.x, lightPos.y, lightPos.z);
Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_col1, 0.1, 0.1, 0.1);
Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_col2, 0.2, 0.2, 0.2);
*/
/*
//Start Render from light perspective
Entity::Primitives::FloorDepthShader.Use();
Entity::Primitives::FloorDepthShader.SetMat4(Entity::Primitives::FloorDepthShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));
glDrawArrays(GL_TRIANGLES, 0, 6);


Entity::Torus::DepthShader.Use();
Entity::Torus::DepthShader.SetMat4(Entity::Torus::DepthShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));


Entity::Primitives::Render<Entity::Torus>();

Entity::Sphere::DepthShader.Use();
Entity::Sphere::DepthShader.SetMat4(Entity::Sphere::DepthShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));

Entity::Primitives::Render<Entity::Sphere>();


// END RENDER FROM LIGHT PERSPECTIVE

glBindFramebuffer(GL_FRAMEBUFFER, 0);


glViewport(0, 0, 800, 600);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



Entity::Primitives::debugDepthQuad.Use();
Entity::Primitives::debugDepthQuad.SetFloat(Entity::Primitives::debugDepthQuad.GetUniformLocation("near_plane"), near_plane);
Entity::Primitives::debugDepthQuad.SetFloat(Entity::Primitives::debugDepthQuad.GetUniformLocation("far_plane"), far_plane);

// render scene as normal 


glBindBuffer(GL_SHADER_STORAGE_BUFFER, Entity::Entity_::GlobalUniforms_SSBO);
glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(RenderCamera.position));
glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(glm::mat4), glm::value_ptr(RenderCamera.GetProjection()));
glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(RenderCamera.GetView()));
glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4), sizeof(unsigned int), &Entity::Lights::Num_DirectionalLights);
glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4) + sizeof(unsigned int), sizeof(unsigned int), &Entity::Lights::Num_PointLights);


Entity::PointLight::UpdateBuffer();
Entity::DirectionalLight::UpdateBuffer();

Entity::Primitives::FloorShader.Use();
//Entity::Primitives::FloorShader.SetMat4(Entity::Primitives::uloc_view, glm::value_ptr(RenderCamera.GetView()));
//Entity::Primitives::FloorShader.SetMat4(Entity::Primitives::uloc_projection, glm::value_ptr(RenderCamera.GetProjection()));
//Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_cam_pos, RenderCamera.position.x, RenderCamera.position.y, RenderCamera.position.y);
Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_col1, 0.1, 0.1, 0.1);
Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_col2, 0.2, 0.2, 0.2);


Entity::Primitives::FloorShader.SetMat4(Entity::Primitives::FloorShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));

Entity::Torus::PrimitiveShader.Use();
Entity::Torus::PrimitiveShader.SetMat4(Entity::Torus::PrimitiveShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));

Entity::Sphere::PrimitiveShader.Use();
Entity::Sphere::PrimitiveShader.SetMat4(Entity::Sphere::PrimitiveShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));


Entity::Primitives::FloorShader.Use();
glActiveTexture(GL_TEXTURE5);
glBindTexture(GL_TEXTURE_2D, Entity::Primitives::depthMap);

Entity::Torus::PrimitiveShader.Use();
glActiveTexture(GL_TEXTURE7);
glBindTexture(GL_TEXTURE_2D, Entity::Primitives::depthMap);

Entity::Sphere::PrimitiveShader.Use();
glActiveTexture(GL_TEXTURE8);
glBindTexture(GL_TEXTURE_2D, Entity::Primitives::depthMap);


Entity::Primitives::debugDepthQuad.Use();



glActiveTexture(GL_TEXTURE6);
glBindTexture(GL_TEXTURE_2D, Entity::Primitives::depthMap);



//Start render from camera perspective
Entity::Primitives::FloorShader.Use();


glDrawArrays(GL_TRIANGLES, 0, 6);

Entity::Torus::PrimitiveShader.Use();
//Entity::Primitives::Render<Entity::Torus>();

Entity::Sphere::PrimitiveShader.Use();
//Entity::Primitives::Render<Entity::Sphere>();

//end render from camera perspective


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

*/

glm::vec3 lightPos = glm::normalize(glm::vec3(0.3, 1.0f, 0.3)) * 35.f;
//glm::vec3 lightDir(-0.707f);t

void Engine::Engine_::Initialize() {
	Window::Initialize();
}

void Engine::Engine_::Render(Camera& RenderCamera) 
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glBindVertexArray(Engine_::u_VAO);

	glm::mat4 view = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

	float near_plane = 0.05f, far_plane = 100.f;

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Entity::Entity_::GlobalUniforms_SSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(lightPos));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(glm::mat4), glm::value_ptr(glm::ortho(-orthosize, orthosize, -orthosize, orthosize, near_plane, far_plane)));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4), sizeof(unsigned int), &Entity::Lights::Num_DirectionalLights);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4) + sizeof(unsigned int), sizeof(unsigned int), &Entity::Lights::Num_PointLights);


	Entity::PointLight::UpdateBuffer();
	Entity::DirectionalLight::UpdateBuffer();


	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	lightProjection = glm::ortho(-orthosize, orthosize, -orthosize, orthosize, near_plane, far_plane);
	lightView = view;
	lightSpaceMatrix = lightProjection * lightView;


	glViewport(0, 0, 4096, 4096);
	glBindFramebuffer(GL_FRAMEBUFFER, Entity::Primitives::depthMapFBO);


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	Entity::Torus::DepthShader.Use();
	Entity::Primitives::Render<Entity::Torus>();

	Entity::Primitives::FloorDepthShader.Use();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	Entity::Sphere::DepthShader.Use();
	Entity::Primitives::Render<Entity::Sphere>();


	// END RENDER FROM LIGHT PERSPECTIVE

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	Entity::Primitives::debugDepthQuad.Use();
	Entity::Primitives::debugDepthQuad.SetFloat(Entity::Primitives::debugDepthQuad.GetUniformLocation("near_plane"), near_plane);
	Entity::Primitives::debugDepthQuad.SetFloat(Entity::Primitives::debugDepthQuad.GetUniformLocation("far_plane"), far_plane);

	// render scene as normal 


	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Entity::Entity_::GlobalUniforms_SSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(RenderCamera.position));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(glm::mat4), glm::value_ptr(RenderCamera.GetProjection()));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(RenderCamera.GetView()));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4), sizeof(unsigned int), &Entity::Lights::Num_DirectionalLights);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4) + sizeof(unsigned int), sizeof(unsigned int), &Entity::Lights::Num_PointLights);


	Entity::PointLight::UpdateBuffer();
	Entity::DirectionalLight::UpdateBuffer();

	Entity::Primitives::FloorShader.Use();
	Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_col1, 0.1, 0.1, 0.1);
	Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_col2, 0.2, 0.2, 0.2);


	Entity::Primitives::FloorShader.SetMat4(Entity::Primitives::FloorShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));

	Entity::Torus::PrimitiveShader.Use();
	Entity::Torus::PrimitiveShader.SetMat4(Entity::Torus::PrimitiveShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));

	Entity::Sphere::PrimitiveShader.Use();
	Entity::Sphere::PrimitiveShader.SetMat4(Entity::Sphere::PrimitiveShader.GetUniformLocation("lightSpaceMatrix"), glm::value_ptr(lightSpaceMatrix));


	Entity::Primitives::FloorShader.Use();
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, Entity::Primitives::depthMap);

	Entity::Torus::PrimitiveShader.Use();
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, Entity::Primitives::depthMap);

	Entity::Sphere::PrimitiveShader.Use();
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, Entity::Primitives::depthMap);


	Entity::Primitives::debugDepthQuad.Use();



	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, Entity::Primitives::depthMap);

	
	

	
	Entity::Torus::PrimitiveShader.Use();
	Entity::Primitives::Render<Entity::Torus>();

	Entity::Primitives::FloorShader.Use();
	glDrawArrays(GL_TRIANGLES, 0, 6);

	Entity::Sphere::PrimitiveShader.Use();
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