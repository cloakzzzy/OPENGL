#pragma once
#include "Camera.hpp"
#include "Torus.hpp"
#include "Window.hpp"

namespace Engine {
	class Window;
	namespace Entity { class Torus; }

	class Engine {
		friend class Entity::Torus;
		inline static unsigned int u_VAO = 20;
		inline static int test = 2003432;
	public:
		static void Initialize();
		static void Render(Camera& cam);
		static Window CreateWindow(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, float Red,
			float Green, float Blue);
	};
	
}



	