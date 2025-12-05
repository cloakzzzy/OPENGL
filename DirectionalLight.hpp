#pragma once
#include <vector>
#include "EntityTemplates.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"

namespace Engine {
	class Window;
	class Engine_;
	namespace Entity {
		class PointLight;
		class DirectionalLight;
		class Primitives;
		class Lights;
		class Entity_;
	}
}

class Engine::Entity::DirectionalLight {
	friend class Entity::EntityAttribute<DirectionalLight>;
	friend class Entity::Entity_;
	friend class Window;
	friend class Engine_;
	friend class Lights;
	friend class Primitives;
	
	glm::vec3 LightPos;
	
	inline static std::vector<unsigned int> ObjectIDs;
	inline constexpr static unsigned int EntitySize = 3;
	inline static constexpr float near_plane = 0.05f, far_plane = 100.f;

	inline static std::vector<glm::mat4*> LightSpaceMatrices;
	inline static std::vector<unsigned int> DepthMaps;

	inline static std::vector<DirectionalLight*> D_Objects;

public:
	inline static std::vector<float> DataBuffer;
	unsigned int Index = 0;

	inline constexpr static size_t MAX_UBO_SIZE = 65536;

	inline static unsigned int SSBO;

	inline static unsigned int HighestShadowMapIndex = 5;
	unsigned int ShadowMapIndex;

	inline static constexpr float orthosize = 60.0f;

	unsigned int ID;

	unsigned int DepthMapFBO;
	unsigned int DepthMap;
	Shader DimpleDepthShader;
	Shader DebugDepthQuad;

	glm::mat4 lightSpaceMatrix;

	unsigned int SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
	
	DirectionalLight(float dir_x, float dir_y, float dir_z);

	static void Initialize();

	static void CreateSSBO();

	void PerFrame(Entity::Camera& RenderCamera);

	void CreateShadowMap();

public:

	EntityAttribute<DirectionalLight> dir_x{ 0, this };
	EntityAttribute<DirectionalLight> dir_y{ 1, this };
	EntityAttribute<DirectionalLight> dir_z{ 2, this };

	static void UpdateBuffer();

	void Delete();
};