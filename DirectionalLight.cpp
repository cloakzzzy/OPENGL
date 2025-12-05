#include "DirectionalLight.hpp"
#include "EntityTemplates.hpp"
#include "Primitives.hpp"
#include "Lights.hpp"

Engine::Entity::DirectionalLight::DirectionalLight(float dir_x, float dir_y, float dir_z) {
   
    Entity_::Generate_ID<DirectionalLight>(ID, Index);

    this->dir_x = dir_x;
    this->dir_y = dir_y;
    this->dir_z = dir_z;

    Entity::Lights::Num_DirectionalLights++;

	LightPos = glm::normalize(glm::vec3(dir_x, dir_y, dir_z)) * 35.f;

	D_Objects.push_back(this);

	CreateShadowMap();
}

void Engine::Entity::DirectionalLight::Delete() {Entity_::DataBuffer_Delete<DirectionalLight>(ID, Index);}

void Engine::Entity::DirectionalLight::CreateSSBO() {

    glGenBuffers(1, &SSBO);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_UBO_SIZE, nullptr, GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);
}

void Engine::Entity::DirectionalLight::CreateShadowMap() {

	DebugDepthQuad.SetFiles("debug_quad.vert", "debug_quad.frag");

	
	glGenFramebuffers(1, &DepthMapFBO);

	// create depth texture
	glGenTextures(1, &DepthMap);
	glBindTexture(GL_TEXTURE_2D, DepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Engine::Entity::DirectionalLight::DepthMaps.push_back(DepthMap);

}

void Engine::Entity::DirectionalLight::PerFrame(Entity::Camera& RenderCamera) {
	glm::mat4 LightView = glm::lookAt(LightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 LightProjection = glm::ortho(-orthosize, orthosize, -orthosize, orthosize, near_plane, far_plane);


	Entity::Entity_::SetGlobalCameraData(LightPos, LightProjection, LightView);


	lightSpaceMatrix = LightProjection * LightView;
	DirectionalLight::LightSpaceMatrices.push_back(&lightSpaceMatrix);
	
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, DepthMapFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Engine::Entity::DirectionalLight::Initialize() {
    CreateSSBO();
}

void Engine::Entity::DirectionalLight::UpdateBuffer() {

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, Entity::Entity_::GlobalUniforms_SSBO);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4), sizeof(unsigned int), &Entity::Lights::Num_DirectionalLights);
	

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, DataBuffer.size() * sizeof(float), &DataBuffer.front());
}