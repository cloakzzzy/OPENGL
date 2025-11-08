#pragma once
#include "Sphere.hpp"
#include "Torus.hpp"

namespace Engine {
	namespace Entity {
		class Torus;
		class Sphere;
		class Primitives {
			inline static int member;
		public:
			static Entity::Torus CreateTorus(float pos_x, float pos_y, float pos_z, float radius, float thickness,
				float red, float green, float blue, float rotx, float roty, float rotz);
			static Entity::Sphere CreateSphere(float pos_x, float pos_y, float pos_z, float radius,
				float red, float green, float blue);

		};
	}
}

