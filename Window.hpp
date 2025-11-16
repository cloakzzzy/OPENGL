#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>
#include <glew.h>
#include "Camera.hpp"

namespace Engine {
	class Engine;
	class Window;
	namespace Entity {
		class Torus; 
		class Sphere;
		class Primitives;
		class PointLight;
	}
}

class Engine::Window {
	friend class Engine;
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
	void MainLoop(function<void()> Content, Camera& cam);

	~Window();
};
