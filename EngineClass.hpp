#pragma once
#include "Camera.hpp"
#include "Torus.hpp"
#include "Window.hpp"

namespace Engine {
	class Window;
	namespace Entity { class Torus;}

	class Engine {
		friend class Entity::Torus;
		friend class Window;
		inline static unsigned int u_VAO;
	public:
		static void Initialize();
		static void Render(Camera& cam);
		static Window CreateWindow(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, float Red,
			float Green, float Blue);
	};
	
}



	