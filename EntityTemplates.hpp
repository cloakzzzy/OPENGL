#pragma once
#include <vector>
#include "Utils.hpp"
#include <iostream>
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include <glew.h>

namespace Engine {
	class Engine_;
	class Window;
	namespace Entity {
		class Entity_;
		class Torus;
		class Sphere;
		template<typename T>
		class EntityAttribute;
		template<typename T>
		class EntityAttribute_Packed;
		class PointLight;
		class DirectionalLight;
		class Lights;
	}
}

class Engine::Entity::Entity_{
	friend class Entity::Torus;
	friend class Entity::Sphere;
	friend class Entity::PointLight;
	friend class Entity::DirectionalLight;
	friend class Window;
	friend class Engine_;

	Entity_();

	inline static unsigned int GlobalUniforms_SSBO;
	
	static void SetGlobalCameraData(glm::vec3 CameraPosition, glm::mat4 CameraProjection, glm::mat4 CameraView) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, GlobalUniforms_SSBO);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(CameraPosition));
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16, sizeof(glm::mat4), glm::value_ptr(CameraProjection));
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(CameraView));
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	template <typename T>
	static void DataBuffer_Add(std::vector<float>& Payload, unsigned int& ID, unsigned int& Index) {
		if (T::ObjectIDs.size() == 0) { ID = 1; }
		else { ID = T::ObjectIDs.back() + 1; }

		T::ObjectIDs.push_back(ID);

		//Inserts the torus vector to the end of the InstanceBuffer
		T::DataBuffer.insert(T::DataBuffer.end(), Payload.begin(), Payload.end());

		//The objects Index is the end TorusIndicesex
		Index = T::ObjectIDs.size() - 1;
	}
	template<typename T>
	static void Generate_ID(unsigned int& ID, unsigned int& Index) {

		for (int i = 0; i < T::EntitySize; i++) { T::DataBuffer.push_back(NULL);}

		if (T::ObjectIDs.size() == 0) { ID = 1; }
		else { ID = T::ObjectIDs.back() + 1; }

		T::ObjectIDs.push_back(ID);

		Index = T::ObjectIDs.size() - 1;
	}

	template<typename T>
	static void DataBuffer_Delete(unsigned int& ID, unsigned int& Index) {
		// Required in case of any instance buffer deletions
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) { Index = BinarySearch(T::ObjectIDs, ID); }
		
		//Required if called on deleted Object
		if (Index == 4294967295) return;

		//Removes object id from Objectid array;
		T::ObjectIDs.erase(T::ObjectIDs.begin() + Index);

		//removes info from instance buffer, stop rendering the torus.
		T::DataBuffer.erase(T::DataBuffer.begin() + Index * T::EntitySize, T::DataBuffer.begin() + Index * T::EntitySize + T::EntitySize);
	
		ID = NULL;
	}
};

template <typename T>
class Engine::Entity::EntityAttribute {
	friend T;

public:
	T* p_EntityObject = nullptr;
	float Value;
	unsigned char Offset;

private:
	__forceinline void Set(unsigned char Offset, T* p_EntityObject, float StartingVal) {
		this->p_EntityObject = p_EntityObject;
		this->Offset = Offset;
		Value = StartingVal;
	}
	EntityAttribute() {};
	EntityAttribute(unsigned char Offset, T* p_EntityObject) : Offset(Offset), p_EntityObject(p_EntityObject) {}

public:
	inline operator float() const { return Value; }

	__forceinline EntityAttribute& operator=(const float NewValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		T::DataBuffer[Index * T::EntitySize + Offset] = NewValue;
		Value = NewValue;
		return *this;
	}

	__forceinline EntityAttribute& operator=(const EntityAttribute& OtherObj) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		float NewValue = OtherObj.Value;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		T::DataBuffer[Index * T::EntitySize + Offset] = NewValue;
		Value = NewValue;
		return *this;
	}

	__forceinline EntityAttribute& operator+=(const float OtherValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		Value += OtherValue;
		T::DataBuffer[Index * T::EntitySize + Offset] = Value;
		return *this;
	}

	__forceinline EntityAttribute& operator-=(const float OtherValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		Value -= OtherValue;
		T::DataBuffer[Index * T::EntitySize + Offset] = Value;

		return *this;
	}

	__forceinline EntityAttribute& operator*=(const float OtherValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		Value *= OtherValue;
		T::DataBuffer[Index * T::EntitySize + Offset] = Value;

		return *this;
	}

	__forceinline EntityAttribute& operator/=(const float OtherValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		Value /= OtherValue;
		T::DataBuffer[Index * T::EntitySize + Offset] = Value;

		return *this;
	}

};

template <typename T>
class Engine::Entity::EntityAttribute_Packed {
	friend T;

public:
	T* p_EntityObject = nullptr;
	unsigned char Offset;
	unsigned char BitOffset;
	unsigned char Value;

private:
	__forceinline void Set(unsigned char Offset,unsigned char BitOffset, T* p_EntityObject, unsigned char StartingVal) {
		this->p_EntityObject = p_EntityObject;
		this->Offset = Offset;
		this->BitOffset = BitOffset;
		Value = StartingVal;
	}
	EntityAttribute_Packed() {};

	EntityAttribute_Packed(unsigned char Offset, unsigned char BitOffset, T* p_EntityObject) : Offset(Offset), BitOffset(BitOffset), p_EntityObject(p_EntityObject) {}

public:
	inline operator float() const { return Value; }
	/*
	__forceinline EntityAttribute_Packed& operator=(const unsigned char NewValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		
		Value = NewValue;

		float& f = T::DataBuffer[Index * T::EntitySize + Offset];
		unsigned int u = std::bit_cast<unsigned int>(f);
		u &= ~(0xFFu << BitOffset);
		u |= (static_cast<unsigned int>(Value) << BitOffset);
		f = std::bit_cast<float>(u);

		return *this;
	}
	*/

	__forceinline EntityAttribute_Packed& operator=(const unsigned char NewValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		Value = NewValue;

		union {float f;unsigned int u;} tmp;

		tmp.f = T::DataBuffer[Index * T::EntitySize + Offset];

		tmp.u &= ~(0xFFu << BitOffset);
		tmp.u |= (static_cast<unsigned int>(Value) << BitOffset);

		T::DataBuffer[Index * T::EntitySize + Offset] = tmp.f;


		return *this;
	}
	__forceinline EntityAttribute_Packed& operator=(const EntityAttribute_Packed& OtherObj) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		float NewValue = OtherObj.Value;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		Value = NewValue;

		union { float f; unsigned int u; } tmp;

		tmp.f = T::DataBuffer[Index * T::EntitySize + Offset];

		tmp.u &= ~(0xFFu << BitOffset);
		tmp.u |= (static_cast<unsigned int>(Value) << BitOffset);

		T::DataBuffer[Index * T::EntitySize + Offset] = tmp.f;

		return *this;


	}

	__forceinline EntityAttribute_Packed& operator+=(const unsigned char OtherValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}

		Value += OtherValue;

		union { float f; unsigned int u; } tmp;

		tmp.f = T::DataBuffer[Index * T::EntitySize + Offset];

		tmp.u &= ~(0xFFu << BitOffset);
		tmp.u |= (static_cast<unsigned int>(Value) << BitOffset);

		T::DataBuffer[Index * T::EntitySize + Offset] = tmp.f;

		return *this;
	}

	__forceinline EntityAttribute_Packed& operator-=(const unsigned char OtherValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		Value -= OtherValue;

		union { float f; unsigned int u; } tmp;

		tmp.f = T::DataBuffer[Index * T::EntitySize + Offset];

		tmp.u &= ~(0xFFu << BitOffset);
		tmp.u |= (static_cast<unsigned int>(Value) << BitOffset);

		T::DataBuffer[Index * T::EntitySize + Offset] = tmp.f;

		return *this;
	}

	__forceinline EntityAttribute_Packed& operator*=(const unsigned char OtherValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		Value *= OtherValue;

		union { float f; unsigned int u; } tmp;

		tmp.f = T::DataBuffer[Index * T::EntitySize + Offset];

		tmp.u &= ~(0xFFu << BitOffset);
		tmp.u |= (static_cast<unsigned int>(Value) << BitOffset);

		T::DataBuffer[Index * T::EntitySize + Offset] = tmp.f;

		return *this;
	}

	__forceinline EntityAttribute_Packed& operator/=(const unsigned char OtherValue) {
		unsigned int& Index = p_EntityObject->Index;
		unsigned int& ID = p_EntityObject->ID;
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) {
			Index = BinarySearch(T::ObjectIDs, ID);
			if (Index == 4294967295) return *this;
		}
		Value /= OtherValue;

		union { float f; unsigned int u; } tmp;

		tmp.f = T::DataBuffer[Index * T::EntitySize + Offset];

		tmp.u &= ~(0xFFu << BitOffset);
		tmp.u |= (static_cast<unsigned int>(Value) << BitOffset);

		T::DataBuffer[Index * T::EntitySize + Offset] = tmp.f;

		return *this;
	}

};




