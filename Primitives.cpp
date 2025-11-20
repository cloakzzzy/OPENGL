#include "Primitives.hpp"
#include "Torus.hpp"
#include "Sphere.hpp"
#include <glew.h>
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"

Engine::Entity::Torus Engine::Entity::Primitives::CreateTorus(float pos_x, float pos_y, float pos_z,
	float radius, float thickness, float red, float green, float blue, float rotx, float roty, float rotz) {
	return Entity::Torus::Torus(pos_x, pos_y, pos_z, radius, thickness, red, green, blue, rotx, roty, rotz);
}

Engine::Entity::Sphere Engine::Entity::Primitives::CreateSphere(float pos_x, float pos_y, float pos_z, float radius, float red, float green, float blue) {
	return Entity::Sphere::Sphere(pos_x, pos_y, pos_z, radius, red, green, blue);
}

void Engine::Entity::Primitives::CreateFloor() {

	glGenBuffers(1, &FloorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, FloorVBO);
	glBufferData(GL_ARRAY_BUFFER, FloorVertices.size() * sizeof(float), &FloorVertices.front(), GL_DYNAMIC_DRAW);

	glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(11);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	FloorShader.SetFiles("FloorShader.vert", "FloorShader.frag");

	FloorShader.Use();

	uloc_view = FloorShader.GetUniformLocation("view");
	uloc_projection = FloorShader.GetUniformLocation("projection");
	uloc_cam_pos = FloorShader.GetUniformLocation("cam_pos");
	uloc_col1 = FloorShader.GetUniformLocation("col1");
	uloc_col2 = FloorShader.GetUniformLocation("col2");

	simpleDepthShader.SetFiles("shadow_mapping_depth.vert", "shadow_mapping_depth.frag");
	debugDepthQuad.SetFiles("debug_quad.vert", "debug_quad.frag");

	// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
	const unsigned int SHADOW_WIDTH = 800, SHADOW_HEIGHT = 600;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//FloorShader.Use();
	//FloorShader.SetInt(FloorShader.GetUniformLocation("shadowMap"), 5);

	debugDepthQuad.Use();
	debugDepthQuad.SetInt(debugDepthQuad.GetUniformLocation("depthMap"), 6);
	
	std::cout << "gdgd" << debugDepthQuad.GetUniformLocation("depthMap") << '\n';

	
}

void Engine::Entity::Primitives::RenderFloor(Camera& cam) {
	
	
	

	
}


