#pragma once

#include "Torus.hpp"

namespace Engine {
	namespace Entity {
		class Torus;
		class Primitives {
			inline static int member;
		public:
			static Entity::Torus CreateTorus(float pos_x, float pos_y, float pos_z, float radius, float thickness,
				float red, float green, float blue, float rotx, float roty, float rotz);
		};
		
	}
}

