#pragma once
#include <vector>
#include "Torus.hpp"

namespace Engine {
	class Engine;
	namespace Entity {
		class Torus;
		class Sphere;
		class Primitives;
	}
	class Window;
}

class Engine::Entity::Primitives {

	friend class Window;
	friend class Engine;
	friend class Torus;

	inline static unsigned int FloorVBO;

	inline static Shader FloorShader;

	inline static unsigned int UBO;

	inline static std::vector<float> FloorVertices{
		10000.0f, 0.f, 10000.f,
		-10000.0f, 0.f, 10000.f,
		-10000.0f, 0.f, -10000.f,
		-10000.0f, 0.f, -10000.f,
		10000.0f, 0.f, -10000.f,
		10000.0f, 0.f, 10000.f,
	};

	static void CreateFloor();
	static void RenderFloor(Camera& cam);

public:
	static Entity::Torus CreateTorus(float pos_x, float pos_y, float pos_z, float radius, float thickness,
		float red, float green, float blue, float rotx, float roty, float rotz);
	static Entity::Sphere CreateSphere(float pos_x, float pos_y, float pos_z, float radius,
		float red, float green, float blue);

};

