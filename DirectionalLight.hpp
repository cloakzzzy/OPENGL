#pragma once
#include <vector>
#include "EntityTemplates.hpp"

namespace Engine {
	class Window;
	class Engine;
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
	friend class Engine;
	friend class Lights;
	

	inline static std::vector<unsigned int> ObjectIDs;
	inline constexpr static unsigned int EntitySize = 3;

	inline static std::vector<float> DataBuffer;
	unsigned int Index = 0;

	inline constexpr static unsigned int MAX_UBO_SIZE = 65536;

	inline static unsigned int SSBO;

	unsigned int ID;


	DirectionalLight(float dir_x, float dir_y, float dir_z);

	static void Initialize();

	static void CreateBuffers();

public:

	EntityAttribute<DirectionalLight> dir_x{ 0, this };
	EntityAttribute<DirectionalLight> dir_y{ 1, this };
	EntityAttribute<DirectionalLight> dir_z{ 2, this };



	static void UpdateBuffer();

	void Delete();
};