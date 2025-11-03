#pragma once

class Torus;

namespace Engine {
	namespace Entity {
		
		class Primitive {
		public:
			static Torus CreateTorus(float pos_x, float pos_y, float pos_z,
				float radius, float thickness,
				float red, float green, float blue, float rotx, float roty, float rotz);
			

		};
	}
	
}