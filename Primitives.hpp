#pragma once
#include <vector>
#include "Shader.hpp"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include "DirectionalLight.hpp"

namespace Engine {
	class Engine_;
	struct Colours;
	struct Colour;
	namespace Entity {
		class Torus;
		class Sphere;
		class Primitives;
		class PointLight;
		class Camera;
		class DirectionalLight;
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
	inline static Shader FloorDepthShader;

	//const static unsigned int SHADOW_WIDTH = 600, SHADOW_HEIGHT = 600;


	inline static Shader FloorShader;

	inline static unsigned int UBO;

	inline static unsigned int uloc_view;
	inline static unsigned int uloc_projection;
	inline static unsigned int uloc_cam_pos;
	inline static unsigned int uloc_col1;
	inline static unsigned int uloc_col2;

	inline static std::vector<float> FloorVertices{
		10000.0f, 0.f, 10000.f,
		-10000.0f, 0.f, -10000.f,
		-10000.0f, 0.f, 10000.f,
		
		10000.0f, 0.f, -10000.f,
		-10000.0f, 0.f, -10000.f,
		
		10000.0f, 0.f, 10000.f,
	};

	template<typename PrimitiveType>

	static void Render() {
		unsigned int NumInstances = PrimitiveType::ObjectIDs.size();

		//lsps
		//shadowmaps

		PrimitiveType::PrimitiveShader.Use();


		// Bind textures to texture units
		for (int i = 0; i < Entity::DirectionalLight::D_Objects.size(); i++) {
			
			GLint uloc_lsp = PrimitiveType::PrimitiveShader.GetUniformLocation("lightSpaceMatrix[" + std::to_string(i) + "]");
			PrimitiveType::PrimitiveShader.SetMat4(uloc_lsp,glm::value_ptr(*(Entity::DirectionalLight::LightSpaceMatrices[i])));

			GLint uloc_smpID = PrimitiveType::PrimitiveShader.GetUniformLocation("shadowMaps[" + std::to_string(i) + "]");
			PrimitiveType::PrimitiveShader.SetInt(uloc_smpID, i);

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, Entity::DirectionalLight::DepthMaps[i]);
		}

		PrimitiveType::GPU_VertexBuffer.Bind();
		PrimitiveType::GPU_ElementBuffer.Bind();

		//puts instance data into IBO
		PrimitiveType::GPU_InstanceBuffer.Bind();
		PrimitiveType::GPU_InstanceBuffer.SetData(PrimitiveType::DataBuffer);

		//Primitive::PrimitiveShader.Use();

		glDrawElementsInstanced(GL_TRIANGLES, PrimitiveType::IndicesData.size(), GL_UNSIGNED_INT, 0, NumInstances);
	}

	template<typename PrimitiveType>

	static void Render_UsingDepthShader() {
		unsigned int NumInstances = PrimitiveType::ObjectIDs.size();

		PrimitiveType::DepthShader.Use();

		PrimitiveType::GPU_VertexBuffer.Bind();
		PrimitiveType::GPU_ElementBuffer.Bind();

		//puts instance data into IBO
		PrimitiveType::GPU_InstanceBuffer.Bind();
		PrimitiveType::GPU_InstanceBuffer.SetData(PrimitiveType::DataBuffer);

		//Primitive::PrimitiveShader.Use();

		glDrawElementsInstanced(GL_TRIANGLES, PrimitiveType::IndicesData.size(), GL_UNSIGNED_INT, 0, NumInstances);
	}

	

	static void CreateFloor();
	static void RenderFloor(Entity::Camera& cam);

public:
	static Entity::Torus CreateTorus(float pos_x, float pos_y, float pos_z, float radius, float thickness,
		float red, float green, float blue, float rot_yaw, float rot_pitch);
	static Entity::Torus CreateTorus(float pos_x, float pos_y, float pos_z, float radius, float thickness,
		Colour colour, float rot_yaw, float rot_pitch);
	static Entity::Sphere CreateSphere(float pos_x, float pos_y, float pos_z, float radius,
		float red, float green, float blue);
	static Entity::Sphere CreateSphere(float pos_x, float pos_y, float pos_z, float radius, Colour colour);

};