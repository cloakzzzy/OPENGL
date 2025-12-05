#include "Torus.hpp"
#include <vector>
#include "Flatshapes.hpp"
#include <glew.h>
#include "EngineClass.hpp"
#include "Camera.hpp"
#include "OpenGLBuffer.hpp"
#include "Primitives.hpp"
#include "EntityTemplates.hpp"
#include "Lights.hpp"
#include <bit>
#include "Colours.hpp"

void Engine::Entity::Torus::GenerateModel(int acc) {
    std::vector<float> verta;
    float th = 360.0f / float(acc);
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
        VertexData.push_back(verta[i]); VertexData.push_back(verta[i + 1]); VertexData.push_back(verta[i + 2]);

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
                
                IndicesData.push_back(start + 1);
                IndicesData.push_back(acc - 1);
                IndicesData.push_back(0);

                IndicesData.push_back(b);
                IndicesData.push_back(start + 1);
         
                IndicesData.push_back(0);
                
                break;
            }
            start = i;
            IndicesData.push_back(i);
            IndicesData.push_back(h);
            IndicesData.push_back(h + 1);

            IndicesData.push_back(i);
            IndicesData.push_back(h + 1);
            IndicesData.push_back(i + 1);

            h++;
            
        }

        else {
            if ((i + 1) % acc != 0) {
                IndicesData.push_back(i + acc);
                IndicesData.push_back(i + acc + 1);
                IndicesData.push_back(i + 1);

                IndicesData.push_back(i + acc); // 1
                IndicesData.push_back(i + 1);  //3
                IndicesData.push_back(i);  //2
                


               

                if ((i + 2) % acc == 0) {
                    
                    IndicesData.push_back(acc * floor((float)i * divide)); // 1,
                    IndicesData.push_back(i + acc + 1); // 3
                    IndicesData.push_back(acc * ceil((float)i * divide));// 2
                    
                    
                    IndicesData.push_back(i + 1); // 1
                    IndicesData.push_back(i + acc + 1); //2 
                    IndicesData.push_back(acc * floor((float)i * divide)); //3
                    
                }
            }
        }
    }
}
void Engine::Entity::Torus::CreateBuffers() {
    GPU_VertexBuffer.CreateBuffer(VertexData.size() * sizeof(float), std::vector<std::pair<unsigned char, unsigned int>>{
        {OpenGLType::Vec3, 5}, 
    });
    GPU_VertexBuffer.SetData(VertexData);

    GPU_ElementBuffer.CreateBuffer(IndicesData.size() * sizeof(float));
    GPU_ElementBuffer.SetData(IndicesData);

    GPU_InstanceBuffer.CreateBuffer(300 * 11 * sizeof(float),
        std::vector<std::pair<unsigned char, unsigned int>>{
            {OpenGLType::Vec3, 7},
            {OpenGLType::Vec2, 8 },
            {OpenGLType::Float, 9 },
            {OpenGLType::Vec2, 10 }}
    );
}


void Engine::Entity::Torus::Initialize() {
    GenerateModel(150);
    CreateBuffers();
    PrimitiveShader.SetFiles("torus.vert", "torus.frag");
    DepthShader.SetFiles("depthshader_torus.vert", "depthshader.frag");
}

Engine::Entity::Torus::Torus(float pos_x, float pos_y, float pos_z,float radius, float thickness,float red, float green, float blue,float rot_yaw, float rot_pitch) {
   
    Entity_::Generate_ID<Torus>(ID, Index);

    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->pos_z = pos_z;

    this->radius = radius;
    this->thickness = thickness;

    this->red = red;
    this->green = green;
    this->blue = blue;

    this->rot_yaw = rot_yaw;
    this->rot_pitch = rot_pitch;
}

void Engine::Entity::Torus::Delete() { Entity_::DataBuffer_Delete<Torus>(ID, Index);}
