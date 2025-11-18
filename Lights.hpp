#pragma once
#include <vector>
#include "Torus.hpp"

namespace Engine {
	namespace Entity {
		class PointLight;
		class Lights;
		class DirectionalLight;
		class Sphere;
		class Torus;
	}	
}

class Engine::Entity::Lights {
	friend class PointLight;
	friend class DirectionalLight;
	friend class Sphere;
	friend class Torus;

	inline static int Num_PointLights = 0;
	inline static int Num_DirectionalLights = 0;
public:
	static PointLight Create_PointLight(float pos_x, float pos_y, float pos_z, float constant, float linear, float quadratic);
	static DirectionalLight Create_DirectionalLight(float dir_x, float dir_y, float dir_z);
};
