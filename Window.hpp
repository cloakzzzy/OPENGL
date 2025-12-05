#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>
#include <glew.h>
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include <string>

namespace Engine {
	class Engine_;
	class Window;
	namespace Entity {
		class Torus; 
		class Sphere;
		class Primitives;
		class PointLight;
		class DirectionalLight;
		class Camera;
	}
}

class Engine::Window {
	friend class Engine_;
private:

	double now;
	double last;
	double freq;
	double DeltaTime = 0.0;

	float VirtualX;
	float VirtualY;

	double monoa = 0;
	bool toggle = true;
	double monob = 0;
	double toggleb = false;

	SDL_Window* WindowObject;

	float Red; float Green; float Blue;

	static void Initialize();
	Window(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, float Red, float Green, float Blue);

	bool IsRunning = true;

public:
	SDL_Event WindowEvents;
	void MainLoop(std::function<void()> Content, Entity::Camera& cam);

	~Window();
};
