#pragma once

#include <vector>
#include "Flatshapes.hpp"
#include "Shader.hpp"
#include <glew.h>
#include "Camera.hpp"
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include "EngineClass.hpp"


namespace Engine {
	class Window;
	namespace Entity {
		class Primitives;
		class Torus {
			friend class Entity::Primitives;
			friend class Engine;
			friend class Window;

		private:
			inline static std::vector<float> InstanceBuffer;
			inline static std::vector<unsigned int> ObjectIDs;
			unsigned int ID;
			unsigned int Index = 0;

			inline static std::vector<float> vert;
			inline static vector<float> cs;
			inline static unsigned int IBO;
			inline static float theta;

			inline static std::vector<unsigned int> ind;
			inline static Shader TorusShader;
			inline static unsigned int VBO;
			inline static unsigned int EBO;
			inline static std::vector<float> offs;
			inline static float num;

			class TorusAttribute {
				friend class Torus;

				Torus* p_TorusObject;
				float Value;
				unsigned char Offset;

				__forceinline void Set(unsigned char Offset, Torus* p_TorusObject, float StartingVal) {
					this->p_TorusObject = p_TorusObject;
					this->Offset = Offset;
					Value = StartingVal;
				}

			public:
				__forceinline operator float() const { return Value; }

				__forceinline TorusAttribute& operator=(const float NewValue) {
					unsigned int& Index = p_TorusObject->Index;
					unsigned int& ID = p_TorusObject->ID;
					if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
						//if these operators are acted upon an object that doesn't exist, then return is -1, which is maxintval because unsigned.
						Index = TorusBinarySearch(ObjectIDs, ID);
						if (Index == 4294967295) return *this;
					}
					InstanceBuffer[Index * 11 + Offset] = NewValue;
					Value = NewValue;
					return *this;
				}

				__forceinline TorusAttribute& operator=(const TorusAttribute& OtherObj) {
					unsigned int& Index = p_TorusObject->Index;
					unsigned int& ID = p_TorusObject->ID;
					float NewValue = OtherObj.Value;
					if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
						Index = TorusBinarySearch(ObjectIDs, ID);
						if (Index == 4294967295) return *this;
					}
					InstanceBuffer[Index * 11 + Offset] = NewValue;
					Value = NewValue;
					return *this;
				}

				__forceinline TorusAttribute& operator+=(const float OtherValue) {
					unsigned int& Index = p_TorusObject->Index;
					unsigned int& ID = p_TorusObject->ID;
					if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID){
						//std::cout << "BINARY SEARCH CALLED FROM += TORUS OPERATOR\n";
						Index = TorusBinarySearch(ObjectIDs, ID);
						if (Index == 4294967295) return *this;
					}
					Value += OtherValue;
					InstanceBuffer[Index * 11 + Offset] = Value;

					return *this;
				}

				__forceinline TorusAttribute& operator-=(const float OtherValue) {
					unsigned int& Index = p_TorusObject->Index;
					unsigned int& ID = p_TorusObject->ID;
					if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
						//std::cout << "BINARY SEARCH CALLED FROM += TORUS OPERATOR\n";
						Index = TorusBinarySearch(ObjectIDs, ID);
						if (Index == 4294967295) return *this;
					}
					Value -= OtherValue;
					InstanceBuffer[Index * 11 + Offset] = Value;

					return *this;
				}

				__forceinline TorusAttribute& operator*=(const float OtherValue) {
					unsigned int& Index = p_TorusObject->Index;
					unsigned int& ID = p_TorusObject->ID;
					if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
						//std::cout << "BINARY SEARCH CALLED FROM += TORUS OPERATOR\n";
						Index = TorusBinarySearch(ObjectIDs, ID);
						if (Index == 4294967295) return *this;
					}
					Value *= OtherValue;
					InstanceBuffer[Index * 11 + Offset] = Value;

					return *this;
				}

				__forceinline TorusAttribute& operator/=(const float OtherValue) {
					unsigned int& Index = p_TorusObject->Index;
					unsigned int& ID = p_TorusObject->ID;
					if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
						//std::cout << "BINARY SEARCH CALLED FROM += TORUS OPERATOR\n";
						Index = TorusBinarySearch(ObjectIDs, ID);
						if (Index == 4294967295) return *this;
					}
					Value /= OtherValue;
					InstanceBuffer[Index * 11 + Offset] = Value;

					return *this;
				}
			};

		public:
			TorusAttribute pos_x;
			TorusAttribute pos_y;
			TorusAttribute pos_z;
			TorusAttribute radius;
			TorusAttribute thickness;
			TorusAttribute red;
			TorusAttribute green;
			TorusAttribute blue;
			TorusAttribute rot_x;
			TorusAttribute rot_y;
			TorusAttribute rot_z;

			void Delete();

		private:
			static void GenerateModel(int acc);
			static void CreateBuffers();

			static int TorusBinarySearch(std::vector<unsigned int>& vec, int target) {
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

			Torus(float pos_x, float pos_y, float pos_z, float radius, float thickness, float red, float green, float blue, float rotx, float roty, float rotz);

			static void Initialize();
			static void Render(Camera& cam);
		};
	}
}

