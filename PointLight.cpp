#include "PointLight.hpp"

Engine::Entity::PointLight::PointLight(float pos_x, float pos_y, float pos_z, float red, float green, float blue) {
    std::vector<float> point_light{
    pos_x,pos_y,pos_z,
    red,green,blue
    };


    if (ObjectIDs.size() == 0) { ID = 1; }
    else { ID = ObjectIDs.back() + 1; }

    ObjectIDs.push_back(ID);

    PointLightData.insert(PointLightData.end(), point_light.begin(), point_light.end());

    Index = ObjectIDs.size() - 1;

    /*
    this->pos_x.Set(0, this, pos_x);
    this->pos_y.Set(1, this, pos_y);
    this->pos_z.Set(2, this, pos_z);

    this->radius.Set(3, this, radius);
    this->thickness.Set(4, this, thickness);

    this->red.Set(5, this, red);
    this->green.Set(6, this, green);
    this->blue.Set(7, this, blue);

    this->rot_x.Set(8, this, rotx);
    this->rot_y.Set(9, this, roty);
    this->rot_z.Set(10, this, rotz);
    */
}

void Engine::Entity::PointLight::Delete() {
    if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) { Index = PointLight_BinarySearch(ObjectIDs, ID); }

    if (Index == 4294967295) return;

    ObjectIDs.erase(ObjectIDs.begin() + Index);

    PointLightData.erase(PointLightData.begin() + Index * 11, PointLightData.begin() + Index * 11 + 11);
}