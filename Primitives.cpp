#include "Primitives.hpp"
#include "Torus.hpp"
#include "Sphere.hpp"
#include <glew.h>
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include "Colours.hpp"
#include "DirectionalLight.hpp"

Engine::Entity::Torus Engine::Entity::Primitives::CreateTorus(float pos_x, float pos_y, float pos_z,
	float radius, float thickness, float red, float green, float blue, float rot_yaw, float rot_pitch) {
	return Entity::Torus::Torus(pos_x, pos_y, pos_z, radius, thickness, red, green, blue, rot_yaw, rot_pitch);
}

Engine::Entity::Torus Engine::Entity::Primitives::CreateTorus(float pos_x, float pos_y, float pos_z,
	float radius, float thickness, Engine::Colour colour, float rot_yaw, float rot_pitch) {
	return Entity::Torus::Torus(pos_x, pos_y, pos_z, radius, thickness, colour.red, colour.green, colour.blue, rot_yaw, rot_pitch);
}

Engine::Entity::Sphere Engine::Entity::Primitives::CreateSphere(float pos_x, float pos_y, float pos_z, float radius, Engine::Colour colour) {
	return Entity::Sphere::Sphere(pos_x, pos_y, pos_z, radius, colour.red, colour.green, colour.blue);
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

	//simpleDepthShader.SetFiles("shadow_mapping_depth.vert", "shadow_mapping_depth.frag");
	
	FloorDepthShader.SetFiles("floorshader_depth.vert", "depthshader.frag");

	FloorShader.Use();
	FloorShader.SetInt(FloorShader.GetUniformLocation("shadowMaps[0]"), 5);

	FloorShader.SetVec3(uloc_col1, 0.1, 0.1, 0.1);
	FloorShader.SetVec3(uloc_col2, 0.2, 0.2, 0.2);


	Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_col1, 0.1, 0.1, 0.1);
	Entity::Primitives::FloorShader.SetVec3(Entity::Primitives::uloc_col2, 0.2, 0.2, 0.2);

	debugDepthQuad.Use();
	debugDepthQuad.SetInt(debugDepthQuad.GetUniformLocation("depthMap"), 6);

	/*
	Entity::Torus::PrimitiveShader.Use();
	Entity::Torus::PrimitiveShader.SetInt(Entity::Torus::PrimitiveShader.GetUniformLocation("shadowMap"), 7);

	Entity::Sphere::PrimitiveShader.Use();
	Entity::Sphere::PrimitiveShader.SetInt(Entity::Sphere::PrimitiveShader.GetUniformLocation("shadowMap"), 8);
	*/


}

void Engine::Entity::Primitives::RenderFloor(Camera& cam) {



}



