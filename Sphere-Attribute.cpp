#include "Sphere.hpp"

void Engine::Entity::Sphere::SphereAttribute::Set(unsigned char Offset, Sphere* p_SphereObject, float StartingVal) {
	this->p_SphereObject = p_SphereObject;
	this->Offset = Offset;
	Value = StartingVal;
}

Engine::Entity::Sphere::SphereAttribute& Engine::Entity::Sphere::SphereAttribute::operator=(const float NewValue) {
	unsigned int& Index = p_SphereObject->Index;
	unsigned int& ID = p_SphereObject->ID;
	if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
		//if these operators are acted upon an object that doesn't exist, then return is -1, which is maxintval because unsigned.
		Index = SphereBinarySearch(ObjectIDs, ID);
		if (Index == 4294967295) return *this;
	}
	InstanceBuffer[Index * 7 + Offset] = NewValue;
	Value = NewValue;
	return *this;
}

Engine::Entity::Sphere::SphereAttribute& Engine::Entity::Sphere::SphereAttribute::operator=(const Sphere::SphereAttribute& OtherObj) {
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

Engine::Entity::Sphere::SphereAttribute& Engine::Entity::Sphere::SphereAttribute::operator+=(const float OtherValue) {
	unsigned int& Index = p_SphereObject->Index;
	unsigned int& ID = p_SphereObject->ID;
	if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
		//std::cout << "BINARY SEARCH CALLED FROM += TORUS OPERATOR\n";
		Index = SphereBinarySearch(ObjectIDs, ID);
		if (Index == 4294967295) return *this;
	}
	Value += OtherValue;
	InstanceBuffer[Index * 7 + Offset] = Value;

	return *this;
}


Engine::Entity::Sphere::SphereAttribute& Engine::Entity::Sphere::SphereAttribute::operator-=(const float OtherValue) {
	unsigned int& Index = p_SphereObject->Index;
	unsigned int& ID = p_SphereObject->ID;
	if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
		//std::cout << "BINARY SEARCH CALLED FROM += TORUS OPERATOR\n";
		Index = SphereBinarySearch(ObjectIDs, ID);
		if (Index == 4294967295) return *this;
	}
	Value -= OtherValue;
	InstanceBuffer[Index * 7 + Offset] = Value;

	return *this;
}

Engine::Entity::Sphere::SphereAttribute& Engine::Entity::Sphere::SphereAttribute::operator*=(const float OtherValue) {
	unsigned int& Index = p_SphereObject->Index;
	unsigned int& ID = p_SphereObject->ID;
	if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
		//std::cout << "BINARY SEARCH CALLED FROM += TORUS OPERATOR\n";
		Index = SphereBinarySearch(ObjectIDs, ID);
		if (Index == 4294967295) return *this;
	}
	Value *= OtherValue;
	InstanceBuffer[Index * 7 + Offset] = Value;

	return *this;
}

Engine::Entity::Sphere::SphereAttribute& Engine::Entity::Sphere::SphereAttribute::operator/=(const float OtherValue) {
	unsigned int& Index = p_SphereObject->Index;
	unsigned int& ID = p_SphereObject->ID;
	if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
		//std::cout << "BINARY SEARCH CALLED FROM += TORUS OPERATOR\n";
		Index = SphereBinarySearch(ObjectIDs, ID);
		if (Index == 4294967295) return *this;
	}
	Value /= OtherValue;
	InstanceBuffer[Index * 7 + Offset] = Value;

	return *this;
}
