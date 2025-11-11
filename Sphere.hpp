#pragma once
#include <vector>
#include "Flatshapes.hpp"
#include "Shader.hpp"
#include <glew.h>
#include "Camera.hpp"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include "OpenGLBuffer.hpp"


namespace Engine {
	class Engine;
	namespace Entity {
		class Primitives;
		class Sphere{
	friend class Entity::Primitives;
	friend class Engine;
	friend class Window;

	inline static std::vector<float> InstanceBuffer;
	inline static std::vector<unsigned int> ObjectIDs;
	unsigned int ID;
	unsigned int Index = 0;

	inline static std::vector<float> vert;
	inline static std::vector<float> Vert;
	inline static OpenGLBuffer IBO;

	inline static std::vector<unsigned int> ind;
	inline static Shader SphereShader;
	inline static unsigned int VBO;
	inline static unsigned int EBO;

	class SphereAttribute {
		friend class Sphere;

		Sphere* p_SphereObject;
		float Value;
		unsigned char Offset;

		__forceinline void Set(unsigned char Offset, Sphere* p_SphereObject, float StartingVal) {
			this->p_SphereObject = p_SphereObject;
			this->Offset = Offset;
			Value = StartingVal;
		}

	public:
		inline operator float() const { return Value; }

		__forceinline SphereAttribute& operator=(const float NewValue) {
			unsigned int& Index = p_SphereObject->Index;
			unsigned int& ID = p_SphereObject->ID;
			if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
				Index = SphereBinarySearch(ObjectIDs, ID);
				if (Index == 4294967295) return *this;
			}
			InstanceBuffer[Index * 7 + Offset] = NewValue;
			Value = NewValue;
			return *this;
		}

		__forceinline SphereAttribute& operator=(const SphereAttribute& OtherObj) {
			unsigned int& Index = p_SphereObject->Index;
			unsigned int& ID = p_SphereObject->ID;
			float NewValue = OtherObj.Value;
			if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
				Index = SphereBinarySearch(ObjectIDs, ID);
				if (Index == 4294967295) return *this;
			}
			InstanceBuffer[Index * 7 + Offset] = NewValue;
			Value = NewValue;
			return *this;
		}

		__forceinline SphereAttribute& operator+=(const float OtherValue) {
			unsigned int& Index = p_SphereObject->Index;
			unsigned int& ID = p_SphereObject->ID;
			if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
				Index = SphereBinarySearch(ObjectIDs, ID);
				if (Index == 4294967295) return *this;
			}
			Value += OtherValue;
			InstanceBuffer[Index * 7 + Offset] = Value;
			return *this;
		}

		__forceinline SphereAttribute& operator-=(const float OtherValue) {
			unsigned int& Index = p_SphereObject->Index;
			unsigned int& ID = p_SphereObject->ID;
			if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
				Index = SphereBinarySearch(ObjectIDs, ID);
				if (Index == 4294967295) return *this;
			}
			Value -= OtherValue;
			InstanceBuffer[Index * 7 + Offset] = Value;
			return *this;
		}

		__forceinline SphereAttribute& operator*=(const float OtherValue) {
			unsigned int& Index = p_SphereObject->Index;
			unsigned int& ID = p_SphereObject->ID;
			if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
				Index = SphereBinarySearch(ObjectIDs, ID);
				if (Index == 4294967295) return *this;
			}
			Value *= OtherValue;
			InstanceBuffer[Index * 7 + Offset] = Value;
			return *this;
		}

		__forceinline SphereAttribute& operator/=(const float OtherValue) {
			unsigned int& Index = p_SphereObject->Index;
			unsigned int& ID = p_SphereObject->ID;
			if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
				Index = SphereBinarySearch(ObjectIDs, ID);
				if (Index == 4294967295) return *this;
			}
			Value /= OtherValue;
			InstanceBuffer[Index * 7 + Offset] = Value;
			return *this;
		}

	};
public:
	SphereAttribute pos_x;
	SphereAttribute pos_y;
	SphereAttribute pos_z;
	SphereAttribute radius;
	SphereAttribute red;
	SphereAttribute green;
	SphereAttribute blue;

	void Delete();
private:
	static void GenerateModel(int acc);
	static void CreateBuffers();

	static int SphereBinarySearch(std::vector<unsigned int>& vec, int target) {
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

	Sphere(float pos_x, float pos_y, float pos_z, float radius, float red, float green, float blue);

	static void Initialize();
	static void Render(Camera& cam);
		};
	}
}
