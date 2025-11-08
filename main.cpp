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
        0.0f, 1.0f, 1.f,
        0.0f, 40.0f, 40.0f);


    cam.position = glm::vec3(0.f, 2.f, 0.f);

    float time = 0.0;

    window.MainLoop([&]() {

       
        torus1.rot_z += 0.125;
        torus1.rot_y += 0.15;

        torus3.pos_x += 0.03f;

        if (time >= 240.0f) {
            torus2.Delete();
            
        }
        else {
            torus2.rot_z += 0.175;
            torus2.rot_y += 0.12;
        }
        time++;
       

        Engine::Engine::Render(cam);
        
    }, cam);


    return 0;
}
