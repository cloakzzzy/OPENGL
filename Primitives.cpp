#include "Primitives.hpp"
#include "Torus.hpp"
#include "Sphere.hpp"
#include <glew.h>

Engine::Entity::Torus Engine::Entity::Primitives::CreateTorus(float pos_x, float pos_y, float pos_z,
	float radius, float thickness,float red, float green, float blue,float rotx, float roty, float rotz) {
	return Entity::Torus::Torus(pos_x, pos_y, pos_z, radius, thickness, red, green, blue, rotx, roty, rotz);
}

Engine::Entity::Sphere Engine::Entity::Primitives::CreateSphere(float pos_x, float pos_y, float pos_z, float radius,float red, float green, float blue) {
	return Entity::Sphere::Sphere(pos_x, pos_y, pos_z, radius, red, green, blue);
}

void Engine::Entity::Primitives::CreateFloor() {

	glGenBuffers(1, &FloorVBO);
	glBindBuffer(GL_ARRAY_BUFFER, FloorVBO);
	glBufferData(GL_ARRAY_BUFFER, FloorVertices.size() * sizeof(float), &FloorVertices.front(), GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(11);

	glBindBuffer(GL_ARRAY_BUFFER,0);

	FloorShader.SetFiles("FloorShader.vert", "FloorShader.frag");

	FloorShader.Use();

	uloc_view = FloorShader.GetUniformLocation("view");
	uloc_projection = FloorShader.GetUniformLocation("projection");
	uloc_cam_pos = FloorShader.GetUniformLocation("cam_pos");
	uloc_col1 = FloorShader.GetUniformLocation("col1");
	uloc_col2 = FloorShader.GetUniformLocation("col2");
	
}

void Engine::Entity::Primitives::RenderFloor(Camera& cam) {
	glBindBuffer(GL_ARRAY_BUFFER, FloorVBO);
	
	FloorShader.Use();
    FloorShader.SetMat4(uloc_view, glm::value_ptr(cam.GetView()));
	FloorShader.SetMat4(uloc_projection, glm::value_ptr(cam.GetProjection()));
    FloorShader.SetVec3(uloc_cam_pos, cam.position.x, cam.position.y, cam.position.z);
	FloorShader.SetVec3(uloc_col1, 0.1, 0.1, 0.1);
    FloorShader.SetVec3(uloc_col2, 0.2, 0.2, 0.2);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

	
	