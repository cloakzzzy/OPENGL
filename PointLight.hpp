#pragma once
#include <vector>

namespace Engine {
	namespace Entity {
		class PointLight;
	}
}

class Engine::Entity::PointLight {
	inline static std::vector<float> PointLightData;
	inline static std::vector<unsigned int> ObjectIDs;

	unsigned int ID;
	unsigned int Index = 0;

	PointLight(float pos_x, float pos_y, float pos_z, float red, float green, float blue);

	static void Initialize();

	static int PointLight_BinarySearch(std::vector<unsigned int>& vec, int target) {
		int left = 0;
		int right = vec.size() - 1;
		while (left <= right) {
			int mid = left + (right - left) * 0.5;
			if (vec[mid] == target)
				return mid;
			if (vec[mid] < target)
				left = mid + 1;
			else
				right = mid - 1;
		}
		return -1;
	}

public:

	static PointLight Create(float pos_x, float pos_y, float pos_z, float red, float green, float blue) {
		return PointLight(pos_x, pos_y, pos_z, red, green, blue);
	}

	void Delete();
};