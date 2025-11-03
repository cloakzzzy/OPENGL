#pragma once
#include "Camera.hpp"

class Torus;

namespace Engine {
	class Engine {
		friend class Torus;
		inline static unsigned int u_VAO;
	public:

		static void Initialize();
		static void Render(Camera& cam);
	};

	
}