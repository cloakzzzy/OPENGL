#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>
#include <glew.h>
#include "Camera.hpp"
#include "Torus.hpp"

namespace Engine {
	class Engine;
	namespace Entity { class Torus; class Sphere; }
	class Window {
		friend class Engine;
	private:

		// --- Timing variables ---
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

		SDL_Event WindowEvents;

		SDL_Window* WindowObject;

		float Red; float Green; float Blue;

		static void Initialize();
		Window(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, float Red, float Green, float Blue);

		bool IsRunning = true;

	public:
		void MainLoop(function<void()> Content, Camera& cam);

		~Window();
	};
}
