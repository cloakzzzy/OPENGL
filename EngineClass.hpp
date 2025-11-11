#pragma once

namespace Engine {
	class Window;
	class Engine;
	namespace Entity { class Torus; class Sphere; }
}

class Engine::Engine {
	friend class Entity::Torus;
	friend class Window;
	inline static unsigned int u_VAO;
public:
	static void Initialize();
	static void Render(Camera& cam);
	static Window CreateWindow(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, float Red,
		float Green, float Blue);
};



	