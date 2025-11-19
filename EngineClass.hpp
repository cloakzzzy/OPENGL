#pragma once

namespace Engine {
	class Window;
	class Engine_;
	namespace Entity { 
		class Torus; 
		class Sphere; 
		class PointLight; 
	}
}

class Engine::Engine_ {
	friend class Entity::Torus;
	friend class Window;
	inline static unsigned int u_VAO;
public:
	static void Initialize();
	static void Render(Camera& cam);
	static Window CreateWindow(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, float Red,
		float Green, float Blue);
};



	