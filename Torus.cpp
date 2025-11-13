#include "Torus.hpp"
#include <vector>
#include "Flatshapes.hpp"
#include <glew.h>
#include "EngineClass.hpp"
#include "Camera.hpp"
#include "OpenGLBuffer.hpp"
#include "Primitives.hpp"
#include "Sphere.hpp"

void Engine::Entity::Torus::GenerateModel(int acc) {
    std::vector<float> verta;
    float th = 360.0f / float(acc);
    theta = th;
    float cx = 0.f;
    float cy = 0.f;
    float cz = 0.f;
    float r = 2.0f;
    float thick = 1.f;

    for (int i = 0; i < acc; i++) {
        std::vector<float> cb = Ngonxz(
            cx, cy, cz,
            cx, cy, cz + r,
            th, i);

        //cs.push_back(cb[0]); cs.push_back(cb[1]); cs.push_back(cb[2]);

        for (int j = 0; j < acc; j++) {
            vector<float> a = Ngonyz(cb[0], cb[1], cb[2], cb[0], cb[1], cb[2] + thick, th, j);
            //rotation
            a = Ngonxz(cb[0], a[1], cb[2], a[0], a[1], a[2], th, i);
            verta.push_back(a[0]); verta.push_back(a[1]); verta.push_back(a[2]);
        }
    }


    int layer;
    int quad;
    float thickness = 0.f;
    float rad = 0.f;
    float theta = 360.0f / acc;

    for (int i = 0; i < verta.size(); i += 3) {
        layer = floor(i / acc / 3.0f);
    }

    int size = verta.size() / 3;


    for (int i = 0; i < verta.size(); i += 3) {
        float layer = floor(i / acc / 3.0f);
        TorusVertices.push_back(verta[i]); TorusVertices.push_back(verta[i + 1]); TorusVertices.push_back(verta[i + 2]);

        //TorusVertices.push_back(cs[layer * 3]);
       // TorusVertices.push_back(layer);
       // TorusVertices.push_back(cs[layer * 3 + 2]);
    }

    int h = 0;
    int start;
    int b;
    bool first = true;
    float divide = 1.0f / acc;

    for (int i = 0; i < size - (acc * (acc - acc)); i++) {
        if (i > ((size - acc)) - 1) {
            if (first == true) {
                b = i;
                first = false;
            }
            if (h == acc - 1) {
                TorusIndices.push_back(start + 1);
                TorusIndices.push_back(acc - 1);
                TorusIndices.push_back(0);

                TorusIndices.push_back(start + 1);
                TorusIndices.push_back(b);
                TorusIndices.push_back(0);

                break;
            }
            start = i;
            TorusIndices.push_back(i);
            TorusIndices.push_back(h);
            TorusIndices.push_back(h + 1);

            TorusIndices.push_back(i);
            TorusIndices.push_back(i + 1);
            TorusIndices.push_back(h + 1);

            h++;
        }

        else {
            if ((i + 1) % acc != 0) {
                TorusIndices.push_back(i + acc);
                TorusIndices.push_back(i + acc + 1);
                TorusIndices.push_back(i + 1);

                TorusIndices.push_back(i + acc);
                TorusIndices.push_back(i);
                TorusIndices.push_back(i + 1);

                if ((i + 2) % acc == 0) {
                    TorusIndices.push_back(acc * floor((float)i * divide));
                    TorusIndices.push_back(acc * ceil((float)i * divide));
                    TorusIndices.push_back(i + acc + 1);

                    TorusIndices.push_back(i + 1);
                    TorusIndices.push_back(i + acc + 1);
                    TorusIndices.push_back(acc * floor((float)i * divide));
                }
            }
        }
    }
}
void Engine::Entity::Torus::CreateBuffers() {


    GPU_VertexBuffer.CreateBuffer(TorusVertices.size() * sizeof(float), std::vector<std::pair<unsigned char, unsigned int>>{
        {OpenGLType::Vec3, 5},
      
    });
    GPU_VertexBuffer.SetData(TorusVertices);

    GPU_ElementBuffer.CreateBuffer(TorusIndices.size() * sizeof(float));
    GPU_ElementBuffer.SetData(TorusIndices);

    GPU_InstanceBuffer.CreateBuffer(300 * 11 * sizeof(float),
        std::vector<std::pair<unsigned char, unsigned int>>{
            {OpenGLType::Vec3, 7},
            {OpenGLType::Vec2, 8 },
            {OpenGLType::Vec3, 9 },
            {OpenGLType::Vec3, 10 }});

    /*
    glGenBuffers(1, &Entity::Primitives::UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, Entity::Primitives::UBO);
    glBufferData(GL_UNIFORM_BUFFER, 65536, nullptr, GL_DYNAMIC_DRAW);
    

    GLuint blockIndex = glGetUniformBlockIndex(TorusShader.ID, "LightData");
    glUniformBlockBinding(TorusShader.ID, blockIndex, 0);      // Block index binding point 0
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, Entity::Primitives::UBO);     // Bind UBO to binding point 0

    GLuint blockIndex2 = glGetUniformBlockIndex(Entity::Sphere::SphereShader.ID, "LightData");
    glUniformBlockBinding(Entity::Sphere::SphereShader.ID, blockIndex, 0);      // Block index binding point 0
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, Entity::Primitives::UBO);     // Bind UBO to binding point 0

    
    size_t numFloats = 65536 / sizeof(float);
    std::vector<float> initData(numFloats, 0.5f);
    float* ptr = (float*)glMapBufferRange(GL_UNIFORM_BUFFER, 0, 65536, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    if (ptr) {
        memcpy(ptr, initData.data(), 65536);
        glUnmapBuffer(GL_UNIFORM_BUFFER);
    }
    */
}


void Engine::Entity::Torus::Initialize() {
    GenerateModel(150);
    CreateBuffers();
    TorusShader.SetFiles("torus.vert", "default.frag");
    TorusShader.Use();
    TorusShader.SetFloat("theta", theta);
}

Engine::Entity::Torus::Torus(float pos_x, float pos_y, float pos_z,
    float radius, float thickness,
    float red, float green, float blue,
    float rotx, float roty, float rotz) {

    vector<float> torus{
    pos_x,pos_y,pos_z,
    radius, thickness,
    red,green,blue,
    rotx,roty,rotz
    };


    if (ObjectIDs.size() == 0) { ID = 1; }
    else { ID = ObjectIDs.back() + 1; }

    ObjectIDs.push_back(ID);

    //Inserts the torus vector to the end of the InstanceBuffer
    InstanceData.insert(InstanceData.end(), torus.begin(), torus.end());

    //The objects Index is the end TorusIndicesex
    Index = ObjectIDs.size() - 1;



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

}

void Engine::Entity::Torus::Delete() {
    // Required in case of any instance buffer deletions
    if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) { Index = TorusBinarySearch(ObjectIDs, ID); }

    //Required if called on deleted Object
    if (Index == 4294967295) return;

    //Removes object id from Objectid array;
    ObjectIDs.erase(ObjectIDs.begin() + Index);

    //removes info from instance buffer, stop rendering the torus.
    InstanceData.erase(InstanceData.begin() + Index * 11, InstanceData.begin() + Index * 11 + 11);

}

void Engine::Entity::Torus::Render(Camera& cam) {
    unsigned int NumInstances = ObjectIDs.size();

    GPU_VertexBuffer.Bind();
    GPU_ElementBuffer.Bind();

    //puts instance data into IBO
    GPU_InstanceBuffer.Bind();
    GPU_InstanceBuffer.SetData(InstanceData);

    TorusShader.Use();
    TorusShader.SetMat4("view", glm::value_ptr(cam.GetView()));
    TorusShader.SetMat4("projection", glm::value_ptr(cam.GetProjection()));

    glDrawElementsInstanced(GL_TRIANGLES, TorusIndices.size(), GL_UNSIGNED_INT, 0, NumInstances);
}


