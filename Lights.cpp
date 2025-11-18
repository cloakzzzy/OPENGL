#include "Lights.hpp"
#include "PointLight.hpp"
#include "DirectionalLight.hpp"

Engine::Entity::PointLight Engine::Entity::Lights::Create_PointLight(float pos_x, float pos_y, float pos_z, float constant, float linear, float quadratic) {
	return Entity::PointLight(pos_x, pos_y, pos_z, constant, linear, quadratic);
}

Engine::Entity::DirectionalLight Engine::Entity::Lights::Create_DirectionalLight(float dir_x, float dir_y, float dir_z) {
	return Entity::DirectionalLight(dir_x, dir_y, dir_z);
}

