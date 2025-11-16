#pragma once
#include <vector>
#include "Flatshapes.hpp"
#include "Shader.hpp"
#include <glew.h>
#include "Camera.hpp"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include "EngineClass.hpp"
#include "OpenGLBuffers.hpp"
#include "EntityTemplates.hpp"
#include "Utils.hpp"

namespace Engine {
	namespace Entity {
		class Primitives;
		class PointLight;
		class Torus;
		class Sphere;
	}
}

class Engine::Entity::Sphere {
	friend class Entity::Primitives;
	friend class Engine;
	friend class Window;
	friend class Torus;
	friend class Entity::Entity_;
	friend class Entity::EntityAttribute<Sphere>;
	friend class Entity::PointLight;

	inline static std::vector<float> DataBuffer;
	inline static std::vector<unsigned int> ObjectIDs;
	inline static std::vector<float> SphereVertices;
	inline static std::vector<unsigned int> SphereIndices;
	inline constexpr static unsigned int EntitySize = 7;

	unsigned int ID;
	unsigned int Index = 0;

	inline static OpenGL_InstanceBuffer IBO;
	inline static OpenGL_VertexBuffer VBO;
	inline static OpenGL_ElementBuffer EBO;

			
	inline static Shader SphereShader;

public:

	EntityAttribute<Sphere> pos_x;
	EntityAttribute<Sphere> pos_y;
	EntityAttribute<Sphere> pos_z;
	EntityAttribute<Sphere> radius;
	EntityAttribute<Sphere> red;
	EntityAttribute<Sphere> green;
	EntityAttribute<Sphere> blue;

	void Delete();

private:
	static void GenerateModel(int acc);
	static void CreateBuffers();
	
	Sphere(float pos_x, float pos_y, float pos_z, float radius, float red, float green, float blue);

	static void Initialize();
	static void Render(Camera& cam);

};