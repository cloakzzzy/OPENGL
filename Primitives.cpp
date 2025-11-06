#include "Primitive.hpp"
#include "Torus.hpp"



Engine::Entity::Torus Engine::Entity::Primitives::CreateTorus(float pos_x, float pos_y, float pos_z,
	float radius, float thickness,
	float red, float green, float blue, float rotx, float roty, float rotz) {

	return Entity::Torus::Torus(pos_x, pos_y, pos_z, radius, thickness, red, green, blue, rotx, roty, rotz);
}
	