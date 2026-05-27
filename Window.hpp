#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>
#include <glew.h>
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include <string>
#include "OpenGLBuffers.hpp"
#include "OpenGLShader.hpp"

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

	float VirtualX;
	float VirtualY;

	inline static OpenGL_VertexBuffer vb;
	inline static OpenGL_ElementBuffer ib;
	//inline static unsigned int vb;
	inline static GLuint vao;


	inline static std::vector<float> v{
		0.5, 0.5f, 0.f,
		0.2f, 0.2f, 0.0f,
		-0.3f, 0.3f, 0.0f
	};

	
	inline static OpenGL_Shader s;

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
	double DeltaTime = 0.0;
	SDL_Event WindowEvents;
	inline static std::vector<float> vertices;
	inline static std::vector<unsigned int> indices;
	void MainLoop(std::function<void()> Content, Entity::Camera& cam);

	~Window();
};
