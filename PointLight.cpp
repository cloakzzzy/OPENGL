#include "PointLight.hpp"
#include "EntityTemplates.hpp"
#include "Lights.hpp"

Engine::Entity::PointLight::PointLight(float pos_x, float pos_y, float pos_z, float constant, float linear, float quadratic) {
    
 

    std::vector<float> point_light{
        pos_x ,pos_y,pos_z,
        constant, linear, quadratic};

    Entity_::DataBuffer_Add<PointLight>(point_light, ID, Index);

    this->pos_x.Set(0, this, pos_x);
    this->pos_y.Set(1, this, pos_y);
    this->pos_z.Set(2, this, pos_z);

    this->constant.Set(3, this, constant);
    this->linear.Set(4, this, linear);
    this->quadratic.Set(5, this, quadratic);

    Entity::Lights::Num_PointLights++;
}

void Engine::Entity::PointLight::Delete() {
    Entity_::DataBuffer_Delete<PointLight>(ID, Index);
}

void Engine::Entity::PointLight::CreateBuffers() {

    /*
   glGenBuffers(1, &UBO);
   glBindBuffer(GL_UNIFORM_BUFFER, UBO);
   glBufferData(GL_UNIFORM_BUFFER, MAX_UBO_SIZE, nullptr, GL_DYNAMIC_DRAW);

   unsigned int blockIndex = glGetUniformBlockIndex(Entity::Torus::TorusShader.ID, "LightData");
   glUniformBlockBinding(Entity::Torus::TorusShader.ID, blockIndex, 0);     
   glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);     

   unsigned int blockIndex2 = glGetUniformBlockIndex(Entity::Sphere::SphereShader.ID, "LightData");
   glUniformBlockBinding(Entity::Sphere::SphereShader.ID, blockIndex, 0);    
   glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);    
   */

   glGenBuffers(1, &SSBO);
   glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
   glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_UBO_SIZE, nullptr, GL_DYNAMIC_DRAW);

   glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);

    
}

void Engine::Entity::PointLight::Initialize() {
    CreateBuffers();
}

void Engine::Entity::PointLight::UpdateBuffer() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, DataBuffer.size() * sizeof(float), &DataBuffer.front());
}