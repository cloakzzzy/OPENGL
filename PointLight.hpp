#pragma once
#include <vector>
#include "EntityTemplates.hpp"

namespace Engine {
	class Window;
	class Engine;
	namespace Entity {
		class PointLight;
		class Torus;
	}
}

class Engine::Entity::PointLight {
	friend class Entity::EntityAttribute<PointLight>;
	friend class Entity::Entity_;
	friend class Window;
	friend class Engine;

	inline static std::vector<unsigned int> ObjectIDs;
	inline constexpr static unsigned int EntitySize = 24;

	inline static std::vector<float> DataBuffer;
	unsigned int Index = 0;
	
	inline constexpr static unsigned int MAX_UBO_SIZE = 65536;

	inline static unsigned int UBO;

	unsigned int ID;
	

	PointLight(float pos_x, float pos_y, float pos_z, float red, float green, float blue);

	static void Initialize();

	static void CreateBuffers();

public:
	
	EntityAttribute<PointLight> pos_x;
	EntityAttribute<PointLight> pos_y;
	EntityAttribute<PointLight> pos_z;
	EntityAttribute<PointLight> red;
	EntityAttribute<PointLight> green;
	EntityAttribute<PointLight> blue;
	

	static PointLight Create(float pos_x, float pos_y, float pos_z, float red, float green, float blue) {
		return PointLight(pos_x, pos_y, pos_z, red, green, blue);
	}

	static void UpdateBuffer();

	void Delete();
};