#include "Window.hpp"
#include <iostream>
#include "Torus.hpp"
#include "Sphere.hpp"
#include "EngineClass.hpp"
#include "Primitives.hpp"

void Engine::Window::Initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "[ENGINE][ERROR]SDL_Init failed: " << SDL_GetError() << "\n";
        return;
    }
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

Engine::Window::Window(std::string WindowTitle, unsigned int ScreenWidth, unsigned int ScreenHeight, float Red, float Green, float Blue) {
    WindowObject = SDL_CreateWindow(WindowTitle.c_str(), ScreenWidth, ScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!WindowObject) {
        std::cerr << "[ENGINE][ERROR] SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(WindowObject);
    if (!glContext) {
        std::cerr << "[ENGINE][ERROR] SDL_GL_CreateContext failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(WindowObject);
        SDL_Quit();
        return;
    }

    glewInit();
    glEnable(GL_DEPTH_TEST);

    // --- Timing variables ---
    now = SDL_GetPerformanceCounter();
    last = 0;
    DeltaTime = 0.0;
    freq = (double)SDL_GetPerformanceFrequency();

    VirtualX = ScreenHeight / 2.0f;
    VirtualY = ScreenWidth / 2.0f;

    this->Red = Red; 
    this->Green = Green;
    this->Blue = Blue;


    SDL_GL_SetSwapInterval(1);

    glGenVertexArrays(1, &Engine::u_VAO);
    glBindVertexArray(Engine::u_VAO);

    Entity::Torus::Initialize();
    Entity::Sphere::Initialize();
    Entity::Primitives::CreateFloor();

}

void Engine::Window::MainLoop(function<void()> Content, Camera& cam) {
    while (IsRunning) {
        last = now;
        now = SDL_GetPerformanceCounter();
        DeltaTime = (double)(now - last) / freq;

        {
            if (SDL_WaitEventTimeout(&WindowEvents, 10)) {
                // We got at least one event — process it
                do {
                    if (WindowEvents.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                        if (WindowEvents.button.button == SDL_BUTTON_RIGHT) {
                            monob += 0.000000001;
                            if (monob == 0.000000001) {
                                toggleb = !toggleb;
                                if (toggleb == false) {
                                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                                }
                                else {
                                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                                }
                            }
                        }
                    }
                    if (WindowEvents.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                        if (WindowEvents.button.button == SDL_BUTTON_RIGHT) {
                            monob = 0;
                        }
                    }
                    if (WindowEvents.type == SDL_EVENT_MOUSE_MOTION) {
                        VirtualX += (float)WindowEvents.motion.xrel;
                        VirtualY += (float)WindowEvents.motion.yrel;
                        cam.Mouse_SetLookAt(VirtualX, VirtualY, 10);

                    }
                    if (WindowEvents.key.key == SDLK_ESCAPE) {
                        monoa += 0.000000001;
                        if (monoa == 0.000000001) {
                            toggle = !toggle;
                            if (toggle == false) {
                                SDL_SetWindowRelativeMouseMode(WindowObject, true);  // reports continuous relative motion, hides/locks cursor for that window
                                SDL_HideCursor();
                            }
                            else {
                                // to return to normal:
                                SDL_SetWindowRelativeMouseMode(WindowObject, false);
                                SDL_ShowCursor();
                            }
                        }
                        if (WindowEvents.type == SDL_EVENT_KEY_DOWN) {
                            if (WindowEvents.key.key == SDLK_ESCAPE) {
                                monoa = 0;
                            }
                        }
                    }

                } while (SDL_PollEvent(&WindowEvents));
            }
            const bool* keys = SDL_GetKeyboardState(NULL);

            float cameraSpeed = static_cast<float>(5.0f) * DeltaTime;

            {
                if (keys[SDL_SCANCODE_W]) {
                    cam.position.x += cameraSpeed * cam.yawdir.x;
                    cam.position.z += cameraSpeed * cam.yawdir.y;
                }
                if (keys[SDL_SCANCODE_S]) {
                    cam.position.x -= cameraSpeed * cam.yawdir.x;
                    cam.position.z -= cameraSpeed * cam.yawdir.y;
                }
                if (keys[SDL_SCANCODE_A]) {
                    cam.position -= glm::normalize(glm::cross(cam.direction, cam.up)) * cameraSpeed;
                }
                if (keys[SDL_SCANCODE_D]) {
                    cam.position += glm::normalize(glm::cross(cam.direction, cam.up)) * cameraSpeed;
                }
                if (keys[SDL_SCANCODE_SPACE]) {
                    cam.position.y += cameraSpeed;
                }
                if (keys[SDL_SCANCODE_LSHIFT]) {
                    cam.position.y -= cameraSpeed;
                }
            }

        }

        glClearColor(Red, Green, Blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Content();

        double fps = 1.0 / DeltaTime;

        //std::cout << "\rFrame time: " << (DeltaTime * 1000.0) << " ms  |  FPS: " << fps << "       " << std::flush;

        SDL_GL_SwapWindow(WindowObject);

     
    }
}

Engine::Window::~Window() {
   SDL_DestroyWindow(WindowObject);
   SDL_Quit();
}