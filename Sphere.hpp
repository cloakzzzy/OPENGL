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
	struct Colour;
	struct Colours;
	namespace Entity {
		class Primitives;
		class PointLight;
		class Torus;
		class Sphere;
		class Entity_;
	}
}

class Engine::Entity::Sphere {
	friend class Entity::Primitives;
	friend class Engine_;
	friend class Window;
	friend class Torus;
	friend class Entity_;
	friend class Entity::EntityAttribute<Sphere>;
	friend class Entity::EntityAttribute_Packed<Sphere>;
	friend class Entity::PointLight;

	inline static std::vector<float> DataBuffer;
	inline static std::vector<unsigned int> ObjectIDs;
	inline static std::vector<float> VertexData;
	inline static std::vector<unsigned int> IndicesData;
	inline constexpr static unsigned int EntitySize = 5;

	unsigned int ID;
	unsigned int Index = 0;

	inline static OpenGL_InstanceBuffer GPU_InstanceBuffer;
	inline static OpenGL_VertexBuffer GPU_VertexBuffer;
	inline static OpenGL_ElementBuffer GPU_ElementBuffer;

			
	inline static Shader PrimitiveShader;
	inline static Shader DepthShader;
	

public:
	EntityAttribute<Sphere> pos_x{0,this};
	EntityAttribute<Sphere> pos_y{1, this };
	EntityAttribute<Sphere> pos_z{2, this };
	EntityAttribute<Sphere> radius{3, this };
	EntityAttribute_Packed<Sphere> red{4, 0, this};
	EntityAttribute_Packed<Sphere> green{4, 8, this };
	EntityAttribute_Packed<Sphere> blue{4, 16, this };

	void Delete();

private:
	static void GenerateModel(int acc);
	static void CreateBuffers();
	unsigned int Colour;
	
	Sphere(float pos_x, float pos_y, float pos_z, float radius, unsigned char red, unsigned char green, unsigned char blue);

	static void Initialize();
	
};