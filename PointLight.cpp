#include "PointLight.hpp"
#include "EntityTemplates.hpp"
#include "Torus.hpp"
#include "Sphere.hpp"
#include "Primitives.hpp"

Engine::Entity::PointLight::PointLight(float pos_x, float pos_y, float pos_z, float red, float green, float blue) {
    
    //padding to adjust for std 140 alightment
    std::vector<float> point_light{
        pos_x ,NULL, NULL, NULL, 
        pos_y, NULL, NULL, NULL,
        pos_z, NULL, NULL, NULL,
        red,  NULL, NULL, NULL,
        green,NULL, NULL, NULL,
        blue, NULL, NULL, NULL};

    Entity_::DataBuffer_Add<PointLight>(point_light, ID, Index);

    this->pos_x.Set(0, this, pos_x);
    this->pos_y.Set(4, this, pos_y);
    this->pos_z.Set(8, this, pos_z);

    this->red.Set(12, this, red);
    this->green.Set(16, this, green);
    this->blue.Set(20, this, blue);

    Entity::Primitives::NumLights++;
}

void Engine::Entity::PointLight::Delete() {
    Entity_::DataBuffer_Delete<PointLight>(ID, Index);
}

void Engine::Entity::PointLight::CreateBuffers() {

   glGenBuffers(1, &UBO);
   glBindBuffer(GL_UNIFORM_BUFFER, UBO);
   glBufferData(GL_UNIFORM_BUFFER, MAX_UBO_SIZE, nullptr, GL_DYNAMIC_DRAW);

   unsigned int blockIndex = glGetUniformBlockIndex(Entity::Torus::TorusShader.ID, "LightData");
   glUniformBlockBinding(Entity::Torus::TorusShader.ID, blockIndex, 0);     
   glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);     

   unsigned int blockIndex2 = glGetUniformBlockIndex(Entity::Sphere::SphereShader.ID, "LightData");
   glUniformBlockBinding(Entity::Sphere::SphereShader.ID, blockIndex, 0);    
   glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);     

    
}

void Engine::Entity::PointLight::Initialize() {
    CreateBuffers();
}

void Engine::Entity::PointLight::UpdateBuffer() {
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, DataBuffer.size() * sizeof(float), &DataBuffer.front());
}