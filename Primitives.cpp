#include "Primitives.hpp"
#include "Torus.hpp"
#include "Sphere.hpp"


Engine::Entity::Torus Engine::Entity::Primitives::CreateTorus(float pos_x, float pos_y, float pos_z,
	float radius, float thickness,
	float red, float green, float blue, float rotx, float roty, float rotz) {
	return Entity::Torus::Torus(pos_x, pos_y, pos_z, radius, thickness, red, green, blue, rotx, roty, rotz);
}

Engine::Entity::Sphere Engine::Entity::Primitives::CreateSphere(float pos_x, float pos_y, float pos_z, float radius,float red, float green, float blue) {
	return Entity::Sphere::Sphere(pos_x, pos_y, pos_z, radius, red, green, blue);
}
	
	