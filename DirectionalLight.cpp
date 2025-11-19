#include "DirectionalLight.hpp"
#include "EntityTemplates.hpp"
#include "Primitives.hpp"
#include "Lights.hpp"

Engine::Entity::DirectionalLight::DirectionalLight(float dir_x, float dir_y, float dir_z) {


    std::vector<float> dir_light{dir_x ,dir_y,dir_z};

    Entity_::DataBuffer_Add<DirectionalLight>(dir_light, ID, Index);

    this->dir_x.Set(0, this, dir_x);
    this->dir_y.Set(1, this, dir_y);
    this->dir_z.Set(2, this, dir_z);


    Entity::Lights::Num_DirectionalLights++;

}

void Engine::Entity::DirectionalLight::Delete() {
    Entity_::DataBuffer_Delete<DirectionalLight>(ID, Index);
}

void Engine::Entity::DirectionalLight::CreateBuffers() {

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

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO);

   
}

void Engine::Entity::DirectionalLight::Initialize() {
    CreateBuffers();
}

void Engine::Entity::DirectionalLight::UpdateBuffer() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, DataBuffer.size() * sizeof(float), &DataBuffer.front());
}