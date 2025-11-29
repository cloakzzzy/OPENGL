# A Demo

<img width="400" height="300" alt="image" src="https://github.com/user-attachments/assets/639706d5-76cc-4358-84dd-a3f2b343037e" />

```cpp
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

using namespace Engine::Entity;


int main() {

    Engine::Engine_::Initialize();

    Engine::Window window = Engine::Engine_::CreateWindow("Engine Window", 800, 600, 0.4, 0.4, 0.4);


    auto torus1 = Primitives::CreateTorus(
        0.0f, 5.0f, -6.0f,
        3.5f, 1.5f,
        255, 0.0f, 0.0f,
        0.0f, 0.0f);

    auto torus2 = Primitives::CreateTorus(
        0.0f, 3.0f, 5.0f,
        2.f, 1.f,
        0.0f, 0.0f, 255,
        0.0f, 0.0f);

    auto torus3 = Primitives::CreateTorus(
        0.0f, 3.0f, 12.0f,
        2.f, 1.f,
        255, 200, 0.0,
        0.0f, 0.0f);

    auto torus4 = Primitives::CreateTorus(
        -5.0f, 4.0f, 3.f,
        2.f, 1.f,
        0.0, 255, 125,
        0.0f, 0.0f);

    auto sphere1 = Primitives::CreateSphere(
        0.0f, 10.0f, 0.0f,
        2.f,
        33, 127, 75);


    auto sphere2 = Primitives::CreateSphere(
        0.0f, 10.0f, 5.f,
        2.f,
        127, 33, 75);


    auto sphere3 = Primitives::CreateSphere(
        0.f, 10.0f, 0.0f,
        2.f,
        0.18, 0.3, 0.55);


    auto sphere4 = Primitives::CreateSphere(
        -5.f, 3.0f, -0.5f,
        1.f,
        255, 125, 255);


    auto sphere5 = Primitives::CreateSphere(
        0.707 * 13.f, 10.0, 0.707 * 13.f,
        1.f,
        255, 125, 255);

    auto sphere6 = Primitives::CreateSphere(
        5.f, 3.0f, -0.5f,
        1.f,
        255, 125, 255);


    auto sphere7 = Primitives::CreateSphere(
        0.0f, 10.f, -5.0f,
        0.25f,
        125, 125, 255);


    auto dlight1 = Lights::Create_DirectionalLight(0.707f, 0.707, 0.707f);



    window.MainLoop([&]() {


        torus1.rot_pitch += 0.525;
        torus1.rot_yaw += 0.55;

        torus2.rot_pitch += 0.725;
        torus2.rot_yaw += 0.5;

        torus3.rot_pitch += 0.45;


        Engine::Engine_::Render(cam);

        }, cam);

    return 0;
}
```
