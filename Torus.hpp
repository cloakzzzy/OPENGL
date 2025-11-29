#pragma once
#include <vector>
#include "Flatshapes.hpp"
#include "Shader.hpp"
#include <glew.h>
#include "Camera.hpp"
#include "OpenGLBuffers.hpp"
#include "Utils.hpp"
#include "EntityTemplates.hpp"


namespace Engine {
	class Window;
	class Engine_;
	namespace Entity {
		class Primitives;
		class Sphere;
		class Torus;
		class PointLight;
		class Entity_;
	}
}

class Engine::Entity::Torus {
	friend class Entity::Primitives;
	friend class Entity::Entity_;
	friend class Engine_;
	friend class Window;
	friend class Entity::EntityAttribute<Torus>; 
	friend class Entity::EntityAttribute_Packed<Torus>;
	friend class Entity::PointLight;
	

private:
	inline static std::vector<float> DataBuffer;
	inline static std::vector<unsigned int> ObjectIDs;

	inline static std::vector<unsigned int> IndicesData;
	inline static std::vector<float> VertexData;

	inline static Shader PrimitiveShader;
	inline static Shader DepthShader;
	
	inline constexpr static unsigned int EntitySize = 8;

	inline static OpenGL_VertexBuffer GPU_VertexBuffer;
	inline static OpenGL_ElementBuffer GPU_ElementBuffer;
	inline static OpenGL_InstanceBuffer GPU_InstanceBuffer;

	unsigned int ID;
	unsigned int Index = 0;

	static void GenerateModel(int acc);
	static void CreateBuffers();
	Torus(float pos_x, float pos_y, float pos_z, float radius, float thickness, 
		float red, float green, float blue, float rot_yaw, float rot_pitch);
	static void Initialize();
	
public:

	EntityAttribute<Torus> pos_x{ 0, this };
	EntityAttribute<Torus> pos_y{ 1, this };
	EntityAttribute<Torus> pos_z{ 2, this };
	EntityAttribute<Torus> radius{ 3, this };
	EntityAttribute<Torus> thickness{ 4, this };
	EntityAttribute_Packed<Torus> red{ 5, 0, this };
	EntityAttribute_Packed<Torus> green{ 5, 8, this };
	EntityAttribute_Packed<Torus> blue{ 5, 16, this };
	EntityAttribute<Torus> rot_yaw{ 6, this };
	EntityAttribute<Torus> rot_pitch{ 7, this };

	void Delete();

	
};

