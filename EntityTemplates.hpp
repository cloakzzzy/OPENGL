#pragma once
#include <vector>
#include "Utils.hpp"
#include <iostream>

namespace Engine {
	namespace Entity {
		class Entity_;
		class Torus;
		class Sphere;
		template<typename T>
		class EntityAttribute;
		class PointLight;
	}
}

class Engine::Entity::Entity_{
	friend class Entity::Torus;
	friend class Entity::Sphere;
	friend class Entity::PointLight;

	Entity_();

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
	static void DataBuffer_Delete(unsigned int& ID, unsigned int& Index) {
		// Required in case of any instance buffer deletions
		if (Index >= T::ObjectIDs.size() or T::ObjectIDs[Index] != ID) { Index = BinarySearch(T::ObjectIDs, ID); }
		
		//Required if called on deleted Object
		if (Index == 4294967295) return;

		//Removes object id from Objectid array;
		T::ObjectIDs.erase(T::ObjectIDs.begin() + Index);

		//removes info from instance buffer, stop rendering the torus.
		T::DataBuffer.erase(T::DataBuffer.begin() + Index * T::EntitySize, T::DataBuffer.begin() + Index * T::EntitySize + T::EntitySize);
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
