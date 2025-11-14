#pragma once
#include <vector>

namespace Engine {
	namespace Entity {
		class Torus;
	}
}


static int BinarySearch(std::vector<unsigned int>& vec, int target) {
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


