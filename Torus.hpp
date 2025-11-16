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
	class Engine;
	namespace Entity {
		class Primitives;
		class Sphere;
		class Torus;
		class PointLight;
		
	}
}

class Engine::Entity::Torus {
	friend class Entity::Primitives;
	friend class Entity::Entity_;
	friend class Engine;
	friend class Window;
	friend class Entity::EntityAttribute<Torus>; 
	friend class Entity::PointLight;
	

private:
	inline static std::vector<float> DataBuffer;
	inline static std::vector<unsigned int> ObjectIDs;

	inline static std::vector<unsigned int> TorusIndices;
	inline static std::vector<float> TorusVertices;

	inline static Shader TorusShader;
	
	inline constexpr static unsigned int EntitySize = 11;

	inline static OpenGL_VertexBuffer GPU_VertexBuffer;
	inline static OpenGL_ElementBuffer GPU_ElementBuffer;
	inline static OpenGL_InstanceBuffer GPU_InstanceBuffer;

	unsigned int ID;
	unsigned int Index = 0;

	static void GenerateModel(int acc);
	static void CreateBuffers();
	Torus(float pos_x, float pos_y, float pos_z, float radius, float thickness, float red, float green, float blue, float rotx, float roty, float rotz);
	static void Initialize();
	static void Render(Camera& cam);

	inline static unsigned int uloc_ViewPos;
	inline static unsigned int uloc_view;
	inline static unsigned int uloc_projection;
	inline static unsigned int uloc_NumLights;
	
public:
	EntityAttribute<Torus> pos_x;
	EntityAttribute<Torus> pos_y;
	EntityAttribute<Torus> pos_z;
	EntityAttribute<Torus> radius;
	EntityAttribute<Torus> thickness;
	EntityAttribute<Torus> red;
	EntityAttribute<Torus> green;
	EntityAttribute<Torus> blue;
	EntityAttribute<Torus> rot_x;
	EntityAttribute<Torus> rot_y;
	EntityAttribute<Torus> rot_z;

	void Delete();

	
};

