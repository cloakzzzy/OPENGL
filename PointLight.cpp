#include "PointLight.hpp"
#include "EntityTemplates.hpp"
#include "Lights.hpp"

Engine::Entity::PointLight::PointLight(float pos_x, float pos_y, float pos_z, float constant, float linear, float quadratic) {

    Entity_::Generate_ID<PointLight>(ID, Index);

    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->pos_z = pos_z;

    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

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

   uint32_t blockIndex = glGetUniformBlockIndex(Entity::Torus::TorusShader.ID, "LightData");
   glUniformBlockBinding(Entity::Torus::TorusShader.ID, blockIndex, 0);     
   glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);     

   uint32_t blockIndex2 = glGetUniformBlockIndex(Entity::Sphere::SphereShader.ID, "LightData");
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
    
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, Entity::Entity_::GlobalUniforms_SSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 16 + 2 * sizeof(glm::mat4) + sizeof(unsigned int), sizeof(unsigned int), &Entity::Lights::Num_PointLights);


    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, DataBuffer.size() * sizeof(float), &DataBuffer.front());
}