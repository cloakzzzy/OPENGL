#pragma once
#include <vector>
#include "EntityTemplates.hpp"

namespace Engine {
	class Window;
	class Engine;
	namespace Entity {
		class PointLight;
		class Primitives;
		class Lights;
		class Entity_;
	}
}

class Engine::Entity::PointLight {
	friend class Entity::EntityAttribute<PointLight>;
	friend class Entity::Entity_;
	friend class Window;
	friend class Engine;
	friend class Lights;

	inline static std::vector<uint32_t> ObjectIDs;
	inline constexpr static uint32_t EntitySize = 6;

	inline static std::vector<float> DataBuffer;
	uint32_t Index = 0;
	
	inline constexpr static uint32_t MAX_UBO_SIZE = 65536;

	inline static uint32_t UBO;
	inline static uint32_t SSBO;

	uint32_t ID;
	

	PointLight(float pos_x, float pos_y, float pos_z, float red, float green, float blue);

	static void Initialize();

	static void CreateBuffers();

public:
	
	EntityAttribute<PointLight> pos_x{ 0, this };
	EntityAttribute<PointLight> pos_y{ 1, this };
	EntityAttribute<PointLight> pos_z{ 2, this };
	EntityAttribute<PointLight> constant{ 3, this };
	EntityAttribute<PointLight> linear{ 4, this };
	EntityAttribute<PointLight> quadratic{ 5, this };
	

	
	static void UpdateBuffer();

	void Delete();
};