#pragma once
#include <vector>
#include "OpenGLShader.hpp"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include "DirectionalLight.hpp"
#include <math.h>

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
	friend class DirectionalLight;

	inline static unsigned int FloorVBO;
	inline static unsigned int depthMapFBO;
	inline static unsigned int depthMap;
	inline static OpenGL_Shader simpleDepthShader;
	inline static OpenGL_Shader debugDepthQuad;
	inline static OpenGL_Shader FloorDepthShader;

	//const static unsigned int SHADOW_WIDTH = 600, SHADOW_HEIGHT = 600;


	inline static OpenGL_Shader FloorShader;

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

		PrimitiveType::Shader.Use();


		// Bind textures to texture units
		for (int i = 0; i < Entity::DirectionalLight::D_Objects.size(); i++) {
			
			GLint uloc_lsp = PrimitiveType::Shader.GetUniformLocation("lightSpaceMatrix[" + std::to_string(i) + "]");
			PrimitiveType::Shader.SetMat4(uloc_lsp,glm::value_ptr(Entity::DirectionalLight::D_Objects[i]->lightSpaceMatrix));

			GLint uloc_smpID = PrimitiveType::Shader.GetUniformLocation("shadowMaps[" + std::to_string(i) + "]");
			PrimitiveType::Shader.SetInt(uloc_smpID, i);

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, Entity::DirectionalLight::DepthMaps[i]);
		}

		PrimitiveType::GPU_VertexBuffer.Bind();
		PrimitiveType::GPU_ElementBuffer.Bind();

		//puts instance data into IBO
		PrimitiveType::GPU_InstanceBuffer.Bind();
		PrimitiveType::GPU_InstanceBuffer.SetData(PrimitiveType::DataBuffer);

		//Primitive::Shader.Use();

		glDrawElementsInstanced(GL_TRIANGLES, PrimitiveType::IndicesData.size(), GL_UNSIGNED_INT, 0, NumInstances);
	}


	template<typename PrimitiveType>
	static void Render_LOD(Camera &cam) {
		unsigned int NumInstances = PrimitiveType::ObjectIDs.size();

		PrimitiveType::Shader.Use();

		// Bind textures to texture units
		for (int i = 0; i < Entity::DirectionalLight::D_Objects.size(); i++) {

			GLint uloc_lsp = PrimitiveType::Shader.GetUniformLocation("lightSpaceMatrix[" + std::to_string(i) + "]");
			PrimitiveType::Shader.SetMat4(uloc_lsp, glm::value_ptr(Entity::DirectionalLight::D_Objects[i]->lightSpaceMatrix));

			GLint uloc_smpID = PrimitiveType::Shader.GetUniformLocation("shadowMaps[" + std::to_string(i) + "]");
			PrimitiveType::Shader.SetInt(uloc_smpID, i);

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, Entity::DirectionalLight::DepthMaps[i]);
		}

		//for one instance
		
		for (int i = 0; i < NumInstances; i++) {
			float pos_x = PrimitiveType::DataBuffer[i * PrimitiveType::EntitySize + 0];
			float pos_y = PrimitiveType::DataBuffer[i * PrimitiveType::EntitySize + 1];
			float pos_z = PrimitiveType::DataBuffer[i * PrimitiveType::EntitySize + 2];

			long double distance = glm::length(cam.position - glm::vec3(pos_x, pos_y, pos_z));
			//std:cout << "d " << PrimitiveType::DataBuffer.size()<<" "<<distance << '\n';

			for (int j = 0; j < PrimitiveType::LOD_DistanceAccuracies.size(); j++) {
				
				if (distance < PrimitiveType::LOD_DistanceAccuracies[j].first) {
					//std::cout << PrimitiveType::LOD_DataBuffers.size() << '\n';
					PrimitiveType::LOD_DataBuffers[j].insert(PrimitiveType::LOD_DataBuffers[j].end(),
						PrimitiveType::DataBuffer.begin() + i * PrimitiveType::EntitySize,
						PrimitiveType::DataBuffer.begin() + i * PrimitiveType::EntitySize + PrimitiveType::EntitySize + 1);
					break;
				}
				
			}
		}
		
		for (int i = 0; i < PrimitiveType::LOD_DistanceAccuracies.size(); i++) {
			PrimitiveType::GPU_VertexBuffers[i].Bind();
			PrimitiveType::GPU_ElementBuffers[i].Bind();

			//puts instance data into IBO
			PrimitiveType::GPU_InstanceBuffers[i].Bind();
			PrimitiveType::GPU_InstanceBuffers[i].SetData(PrimitiveType::LOD_DataBuffers[i]);

			glDrawElementsInstanced(GL_TRIANGLES, PrimitiveType::IndicesData[i].size(), GL_UNSIGNED_INT, 0, PrimitiveType::LOD_DataBuffers[i].size() / PrimitiveType::EntitySize);
		}

		for (auto i : PrimitiveType::LOD_DataBuffers) {
			i.clear();
		}
		
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

		//Primitive::Shader.Use();

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