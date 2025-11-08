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
    
    
    auto torus1 = Primitives::CreateTorus(0.0f, 0.0f, 0.0f,
        1.f, 1.f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f);

    auto torus2 = Primitives::CreateTorus(0.0f, 0.0f, 5.0f,
        2.f, 1.f,
        0.0f, 0.0f, 1.f,
        0.0f, 0.0f, 0.0f);

    
    auto torus3 = Primitives::CreateTorus(
        0.0f, 7.0f, 5.0f,
        2.f, 1.f,
        0.0f, 0.0f, 0.f,
        0.0f, 40.0f, 40.0f);
    
    auto sphere1 = Primitives::CreateSphere(
        3.0f, 7.0f, 5.0f,
        2.f,
        0.5f, 1.0f, 0.f);
        
    cam.position = glm::vec3(0.f, 0.f, 0.f);

    long long nums;

    
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    

    float time = 0.0;
    
    window.MainLoop([&]() {
        
        
        torus1.rot_z += 0.525;
        torus1.rot_y += 0.55;

        torus2.rot_z += 0.525;
        torus2.rot_y += 0.15;

        torus3.red = sin(time/10);
        torus3.green = sin(time / 10) * 0.1;
        torus3.blue = sin(time / 10) * 0.3;

        sphere1.pos_x += 0.025f;
        
        time++;
        

        Engine::Engine::Render(cam);
        
    }, cam);
    
    return 0;
}
