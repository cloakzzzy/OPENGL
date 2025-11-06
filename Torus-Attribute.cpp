#include "Torus.hpp"

void Engine::Entity::Torus::TorusAttribute::Set(unsigned char Offset, Torus* p_TorusObject, float StartingVal) {
	this->p_TorusObject = p_TorusObject;
	this->Offset = Offset;
	Value = StartingVal;
}

Engine::Entity::Torus::TorusAttribute& Engine::Entity::Torus::TorusAttribute::operator=(const float NewValue) {
	unsigned int &Index = p_TorusObject->Index;
	unsigned int &ID = p_TorusObject->ID;
	if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
	//if these operators are acted upon an object that doesn't exist, then return is -1, which is maxintval because unsigned.
		Index = TorusBinarySearch(ObjectIDs, ID);
	}
	InstanceBuffer[Index * 11 + Offset] = NewValue;
	Value = NewValue;
	return *this;
}

Engine::Entity::Torus::TorusAttribute& Engine::Entity::Torus::TorusAttribute::operator=(const Torus::TorusAttribute& OtherObj) {
	unsigned int &Index = p_TorusObject->Index;
	unsigned int &ID = p_TorusObject->ID;
	float NewValue = OtherObj.Value;
	if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
		Index = TorusBinarySearch(ObjectIDs, ID);
	}
	InstanceBuffer[Index * 11 + Offset] = NewValue;
	Value = NewValue;
	return *this;
}

Engine::Entity::Torus::TorusAttribute& Engine::Entity::Torus::TorusAttribute::operator+=(const float OtherValue) {
	unsigned int &Index = p_TorusObject->Index;
	unsigned int &ID = p_TorusObject->ID;
	if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
		Index = TorusBinarySearch(ObjectIDs, ID);
		if (Index == 4294967295) return *this;
	}
	Value += OtherValue;
	InstanceBuffer[Index * 11 + Offset] = Value;
	
	return *this;
}




