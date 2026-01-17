# A Demo of The Rendering Engine

The following code renders four toruses and six spheres casting shadows upon a greyscale checkerboard floor, illuminated by a directional light.

    #include <SDL3/SDL.h>
    #include <SDL3/SDL_mouse.h>
    #include <glew.h>  
    #include <iostream>
    #include "Engine.hpp"
    #include <chrono>

    constexpr uint32_t SCR_WIDTH = 800;
    constexpr uint32_t SCR_HEIGHT = 600;

    using namespace Engine::Entity;


    int main() {

        Engine::Engine_::Initialize();

        Engine::Window window = Engine::Engine_::CreateWindow("Engine Window", 800, 600, 0.4, 0.4, 0.4);

        auto camera = Engine::Entity::Camera::CreateCamera(SCR_WIDTH, SCR_HEIGHT, 90, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.2, 100);

        auto torus1 = Primitives::CreateTorus(
            0.0f, 5.0f, -6.0f,
            3.5f, 1.5f,
            Engine::Colours::DarkGray,
            0.0f, 0.0f);

        auto torus2 = Primitives::CreateTorus(
            0.0f, 3.0f, 5.0f,
            2.f, 1.f,
            Engine::Colours::LimeGreen,
            0.0f, 0.0f);

        auto torus3 = Primitives::CreateTorus(
            0.0f, 3.0f, 12.0f,
            2.f, 1.f,
            Engine::Colours::RosyBrown,
            0.0f, 0.0f);

        auto torus4 = Primitives::CreateTorus(
            -5.0f, 6.f, 3.f,
            2.f, 1.f,
            Engine::Colours::LightSeaGreen,
            34.f, 256.f);

        auto sphere1 = Primitives::CreateSphere(
            0.0f, 10.0f, 0.0f,
            2.f,
            Engine::Colours::IndianRed);


        auto sphere2 = Primitives::CreateSphere(
            0.0f, 10.0f, 5.f,
            2.f,
            Engine::Colours::Tomato);


        auto sphere3 = Primitives::CreateSphere(
            0.f, 10.0f, 0.0f,
            2.f,
            Engine::Colours::Azure);


        auto sphere4 = Primitives::CreateSphere(
            -5.f, 3.0f, -0.5f,
            1.f,
            Engine::Colours::Bisque);


        auto sphere5 = Primitives::CreateSphere(
            0.707 * 13.f, 10.0, 0.707 * 13.f,
            1.f,
            Engine::Colours::Aqua);


        auto sphere6 = Primitives::CreateSphere(
            5.f, 3.0f, -0.5f,
            1.f,
            Engine::Colours::MistyRose);

        auto sphere7 = Primitives::CreateSphere(
            0.0f, 10.f, -5.0f,
            1.0f,
            Engine::Colours::Coral);


        auto dlight1 = Lights::Create_DirectionalLight(0.707f, 0.707f, 0.707f);




        window.MainLoop([&]() {


            torus1.rot_pitch += 0.525;
            torus1.rot_yaw += 0.55;

            torus2.rot_pitch += 0.725;
            torus2.rot_yaw += 0.5;

            torus3.rot_pitch += 0.45;


            Engine::Engine_::Render(camera);

            }, camera);

        return 0;
    } 

<img width="596" height="442" alt="image" src="https://github.com/user-attachments/assets/5d696c3e-c310-4a80-9c9f-2f0bbb4a01f6" />
