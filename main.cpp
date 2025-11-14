#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>
#include <glew.h>  
#include <iostream>
#include "Engine.hpp"
#include "Camera.hpp"
#include <chrono>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera cam(SCR_WIDTH, SCR_HEIGHT, 90, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.2, 100);


using Engine::Entity::Primitives;

int main() {


    Engine::Engine::Initialize();
    
    Engine::Window window = Engine::Engine::CreateWindow("Engine Window", 800, 600, 0.53, 0.81, 0.92);

    auto torus1 = Primitives::CreateTorus(
        0.0f, 3.0f, 0.0f,
        1.f, 1.f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f);

    auto torus2 = Primitives::CreateTorus(
        5.0f, 3.0f, 5.0f,
        2.f, 1.f,
        0.0f, 0.0f, 1.f,
        0.0f, 0.0f, 0.0f);

    auto torus3 = Primitives::CreateTorus(
        0.0f, 3.0f, 12.0f,
        2.f, 1.f,
        1.0, 0.75, 0.0,
        0.0f, 0.0f, 40.0f);
    
    auto sphere1 = Primitives::CreateSphere(
        0.0f, 10.0f, 0.0f,
        2.f,
        0.18, 0.55, 0.3);

    auto sphere2 = Primitives::CreateSphere(
        0.0f, 10.0f, 5.0f,
        2.f,
        1.f, 1.f, 0.0f);
        
   
    
    float sum_duration = 0;
    
    
    for(int j = 0; j < 10; j++){
        long long nums;
        cin >> nums;
        auto start = std::chrono::high_resolution_clock::now();

        for (long long i = 0; i < nums; i++) {
            torus1.pos_y += torus2.pos_y;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

        sum_duration+=duration;

        std::cout << duration << " ms\n";
    }

    std::cout << (float)sum_duration / 10.f << '\n';
    
    
    float time = 0.0;
    
    window.MainLoop([&]() {
        
        torus1.rot_z += 0.525;
        torus1.rot_y += 0.55;

        torus2.rot_z += 0.725;
        torus2.rot_y += 0.5;

        torus3.rot_z += 0.45;
        torus3.rot_y += 0.7;

        sphere1.pos_x += 0.01;
        sphere1.pos_y += 0.03;

        
        time++;

        Engine::Engine::Render(cam);
        
    }, cam);
    
    return 0;
}
