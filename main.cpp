#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>
#include <glew.h>  
#include <iostream>
#include "Engine.hpp"
#include "Camera.hpp"
#include <chrono>
#include <glew.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera cam(SCR_WIDTH, SCR_HEIGHT, 90, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.2, 100);

//using namespace Engine;
using namespace Engine::Entity;


int main() {

    Engine::Engine_::Initialize();
    
    //Engine::Window window = Engine::Engine::CreateWindow("Engine Window", 800, 600, 0.53, 0.81, 0.92);
    Engine::Window window = Engine::Engine_::CreateWindow("Engine Window", 800, 600, 0.4,0.4,0.4);

    
    auto torus1 = Primitives::CreateTorus(
        0.0f, 5.0f, -6.0f,
        3.5f, 1.5f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f);

    auto torus2 = Primitives::CreateTorus(
        0.0f, 3.0f, 5.0f,
        2.f, 1.f,
        0.0f, 0.0f, 1.f,
        0.0f, 0.0f, 0.0f);

    auto torus3 = Primitives::CreateTorus(
        0.0f, 3.0f, 12.0f,
        2.f, 1.f,
        1.0, 0.75, 0.0,
        0.0f, 0.0f, 0.0f);
    
    auto torus4 = Primitives::CreateTorus(
        -5.0f, 4.0f, 3.f,
        2.f, 1.f,
        0.0, 1.0, 0.50,
        0.0f, 0.0f, 0.0f);
    
    auto sphere1 = Primitives::CreateSphere(
        0.0f, 10.0f, 0.0f,
        2.f,
        0.18, 0.55, 0.3);

    auto sphere2 = Primitives::CreateSphere(
        0.0f, 10.0f, 5.f,
        2.f,
        0.55, 0.18, 0.3);
    
    
    auto sphere3 = Primitives::CreateSphere(
        0.f, 10.0f, 0.0f,
        2.f,
        0.18, 0.3, 0.55);

    auto sphere4 = Primitives::CreateSphere(
        0.707 * 20.f, 0.707 * 20.f, 0.707 * 20.f,
        1.f,
        1.0f, 1.0f, 1.0f);

    auto sphere5 = Primitives::CreateSphere(
        -5.f, 3.0f, -0.5f,
        1.f,
        1.0f, 0.5, 1.0f);

    /*
    auto sphere6 = Primitives::CreateSphere(
        0.707 * 13.f, 0.707 * 13.f, 0.707 * 13.f,
        1.f,
        1.0f, 0.5, 1.0f);
        */

    auto sphere6 = Primitives::CreateSphere(
        5.f, 3.0f, -0.5f,
        1.f,
        1.0f, 0.5, 1.0f);
        
    /*
    auto light_sphere1 = Primitives::CreateSphere(
        0.0f, 10.f, 5.0f,
        0.25f,
        1.0f, 1.0f, 1.0f);
    
    
    auto light_sphere2 = Primitives::CreateSphere(
        0.0f, 10.f, -5.0f,
        0.25f,
        1.0f, 1.0f, 1.0f);
   
    auto light1 = Lights::Create_PointLight(0.0f, 10.f, 5.0f, 1.0,0.045, 0.0075);
    auto light2 = Lights::Create_PointLight(0.0f, 10.f, -5.0f, 1.0, 0.045, 0.0075);
    */

    auto dlight1 = Lights::Create_DirectionalLight(0.707f, 0.707, 0.707f);

    /*
    float sum_duration = 0;
    for(int j = 0; j < 10; j++){
        long long nums;
        cin >> nums;
        auto start = std::chrono::high_resolution_clock::now();

        for (long long i = 0; i < nums; i++) {
            torus1.pos_y += torus3.pos_z;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        sum_duration+=duration;

        std::cout << duration << " ms\n";
    }

    std::cout << (float)sum_duration / 10.f << '\n';
    */

    
    float time = 0;

     
    window.MainLoop([&]() {

        
        torus1.rot_z += 0.525;
        torus1.rot_y += 0.55;

        torus2.rot_z += 0.725;
        torus2.rot_y += 0.5;

        torus3.rot_z += 0.45;
        torus3.rot_y += 0.7;


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

        
        Engine::Engine_::Render(cam);


        time+=0.02;
        
    }, cam);
    
    return 0;
}
