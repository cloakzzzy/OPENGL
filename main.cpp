#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>
#include <glew.h>  
#include <iostream>
#include "Engine.hpp"
#include "Camera.hpp"
#include <chrono>
#include <glew.h>

const uint32_t SCR_WIDTH = 800;
const uint32_t SCR_HEIGHT = 600;

Camera cam(SCR_WIDTH, SCR_HEIGHT, 90, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.05, 100.f);

//using namespace Engine;
using namespace Engine::Entity;


int main() {

    Engine::Engine_::Initialize();

    //Engine::Window window = Engine::Engine::CreateWindow("Engine Window", 800, 600, 0.53, 0.81, 0.92);
    Engine::Window window = Engine::Engine_::CreateWindow("Engine Window", 800, 600, 0.4, 0.4, 0.4);

    

    /*
    auto torus1 = Primitives::CreateTorus(
        0.0f, 5.0f, -6.0f,
        3.5f, 1.5f,
        222, 111, 100,
        0.0f, 0.0f);

    auto torus2 = Primitives::CreateTorus(
        0.0f, 3.0f, 5.0f,
        2.f, 1.f,
        234, 156, 1.f,
        0.0f, 0.0f);

    auto torus3 = Primitives::CreateTorus(
        0.0f, 3.0f, 12.0f,
        2.f, 1.f,
        255, 0.75, 0.0,
        0.0f, 0.0f);


    auto torus4 = Primitives::CreateTorus(
        -5.0f, 4.0f, 3.f,
        2.f, 1.f,
        234, 1.0, 200,
        0.0f, 0.0f);
    */

    int a = 0;
 

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            auto sphere = Primitives::CreateSphere(i * 2, 10.f, j * 2, 1.0f, 128.0f, 128.0f, 0.f);
            sphere.blue = 245;
        }
    }

    auto Torus = Primitives::CreateTorus(
        0, 15.f, 0,
        2.0f, 1.0f,
        0.f, 0.f, 0.f,
        0.f, 245.f);

    Torus.blue += 253;
    //Torus.green = 255;

    glm::vec3 lp = normalize(glm::vec3(0.3, 1.0f, 0.3)) * 35.f;

    
    
    auto dlight1 = Lights::Create_DirectionalLight(0.3f, 1.0f, 0.3f);

    
    float time = 0;



    window.MainLoop([&]() {

        /*
          torus1.rot_pitch += 0.525;
        torus1.rot_yaw += 0.55;

       torus2.rot_pitch += 0.725;
        torus2.rot_yaw += 0.5;

        torus3.rot_pitch += 0.45;
        torus3.rot_yaw += 0.7;

        torus3.rot_pitch += 0.01f;
        torus3.rot_yaw += 0.01f;
       */
       /*
       light1.pos_x = 5.0f * cos(time);
       light1.pos_z = 5.0f * sin(time);

       light_sphere1.pos_x = 5.0f * cos(time);
       light_sphere1.pos_z = 5.0f * sin(time);

       light2.pos_x = 10.f * cos(time + 0.3) - 0.0f;
       light2.pos_z = 10.f * sin(time + 0.2) + 3.0f;

       light_sphere2.pos_x = 10.0f * cos(time + 0.3) - 0.0f;
       light_sphere2.pos_z = 10.0f * sin(time + 0.2) + 3.0f;
       */

        Torus.rot_pitch += 0.525;
        Torus.rot_yaw += 0.55;

        //Torus.pos_x += 0.02f;

        Engine::Engine_::Render(cam);


        time += 0.02;

        }, cam);

    return 0;
}
