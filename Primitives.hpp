#pragma once
#include <vector>
#include "Torus.hpp"

namespace Engine {
	class Engine;
	namespace Entity {
		class Torus;
		class Sphere;
		class Primitives;
		class PointLight;
	}
	class Window;
}

class Engine::Entity::Primitives {

	friend class Window;
	friend class Engine;
	friend class Torus;
	friend class PointLight;
	friend class Sphere;

	inline static unsigned int FloorVBO;

	inline static int NumLights = 0;

	inline static Shader FloorShader;

	inline static unsigned int UBO;

	inline static unsigned int uloc_view;
	inline static unsigned int uloc_projection;
	inline static unsigned int uloc_cam_pos;
	inline static unsigned int uloc_col1;
	inline static unsigned int uloc_col2;

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

