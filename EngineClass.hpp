#pragma once
#include "Camera.hpp"
#include "Torus.hpp"
#include "Engine.hpp"

namespace Engine {
	namespace Entity { class Torus;  }
	class Engine {
		friend class Entity::Torus;
		inline static unsigned int u_VAO;
	public:
		static void Initialize();
		static void Render(Camera& cam);
	};
}
	