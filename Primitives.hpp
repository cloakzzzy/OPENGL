#pragma once
#include <vector>
#include "Torus.hpp"

namespace Engine {
	class Engine_;
	namespace Entity {
		class Torus;
		class Sphere;
		class Primitives;
		class PointLight;
	}
	class Window;
}

class Engine::Entity::Primitives {

	friend class Window;
	friend class Engine_;
	friend class Torus;
	friend class PointLight;
	friend class Sphere;

	inline static unsigned int FloorVBO;
	inline static unsigned int depthMapFBO;
	inline static unsigned int depthMap;
	inline static Shader simpleDepthShader;
	inline static Shader debugDepthQuad;


	const static unsigned int SHADOW_WIDTH = 600, SHADOW_HEIGHT = 600;


	inline static Shader FloorShader;

	inline static unsigned int UBO;

	inline static unsigned int uloc_view;
	inline static unsigned int uloc_projection;
	inline static unsigned int uloc_cam_pos;
	inline static unsigned int uloc_col1;
	inline static unsigned int uloc_col2;

	inline static std::vector<float> FloorVertices{
		10000.0f, 0.f, 10000.f,
		-10000.0f, 0.f, 10000.f,
		-10000.0f, 0.f, -10000.f,
		-10000.0f, 0.f, -10000.f,
		10000.0f, 0.f, -10000.f,
		10000.0f, 0.f, 10000.f,
	};

	template<typename Primitive>
	
	static void Render() {
		unsigned int NumInstances = Primitive::ObjectIDs.size();

		Primitive::GPU_VertexBuffer.Bind();
		Primitive::GPU_ElementBuffer.Bind();

		//puts instance data into IBO
		Primitive::GPU_InstanceBuffer.Bind();
		Primitive::GPU_InstanceBuffer.SetData(Primitive::DataBuffer);

		Primitive::PrimitiveShader.Use();

		glDrawElementsInstanced(GL_TRIANGLES, Primitive::IndicesData.size(), GL_UNSIGNED_INT, 0, NumInstances);
	}
	
	static void CreateFloor();
	static void RenderFloor(Camera& cam);

public:
	static Entity::Torus CreateTorus(float pos_x, float pos_y, float pos_z, float radius, float thickness,
		float red, float green, float blue, float rotx, float roty, float rotz);
	static Entity::Sphere CreateSphere(float pos_x, float pos_y, float pos_z, float radius,
		float red, float green, float blue);

};
