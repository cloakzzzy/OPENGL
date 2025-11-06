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
#include "Engine.hpp"


namespace Engine {
	namespace Entity {
		class Primitives;

		class Torus {
			friend class Entity::Primitives;
			friend class Engine::Engine;

		public:
			inline static std::vector<float> InstanceBuffer;
			inline static std::vector<unsigned int> ObjectIDs;
			unsigned int ID;
			unsigned int Index = 0;
		private:

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

				void Set(unsigned char Offset, Torus* p_TorusObject, float StartingVal);

			public:
				inline operator float() const { return Value; }

				TorusAttribute& operator=(const float NewValue);
				TorusAttribute& operator=(const TorusAttribute& OtherObj);
				TorusAttribute& operator+=(const float OtherValue);

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

			int a = 0;

			void Delete();


		private:
			static void GenerateModel(int acc);
			static void CreateBuffers();

			static int TorusBinarySearch(std::vector<unsigned int>& vec, int target) {
				int left = 0;
				int right = vec.size() - 1;
				while (left <= right) {
					int mid = left + (right - left) * 0.5;
					if (vec[mid] == target) //right == left
						return mid;
					if (vec[mid] < target) // ignore right
						left = mid + 1;
					else // ignore left
						right = mid - 1;
				}
				return -1;
			}

			Torus(float pos_x, float pos_y, float pos_z,
				float radius, float thickness,
				float red, float green, float blue,
				float rotx, float roty, float rotz);

			static void Initialize();

			static void Render(Camera& cam);
		};

	}

}


