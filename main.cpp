#include <SDL3/SDL.h>
#include <SDL3/SDL_mouse.h>
#include <glew.h>  
#include <iostream>
#include "Engine.hpp"
#include "Camera.hpp"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera cam(SCR_WIDTH, SCR_HEIGHT, 90, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.2, 100);


using Engine::Entity::Primitives;

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return -1;
    }

    // Request OpenGL attributes (for example OpenGL 3.3 core)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Create window with OpenGL context
    SDL_Window* window = SDL_CreateWindow(
        "SDL3 + OpenGL Demo",
        SCR_WIDTH, SCR_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return -1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    glewInit();
    
    glEnable(GL_DEPTH_TEST);

    Engine::Engine::Initialize();
    auto torus1 = Primitives::CreateTorus(0.0f, 0.0f, 0.0f,
        1.f, 1.f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f);

    auto torus2 = Primitives::CreateTorus(0.0f, 0.0f, 5.0f,
        2.f, 1.f,
        0.0f, 0.0f, 1.f,
        0.0f, 0.0f, 0.0f);

    SDL_GL_SetSwapInterval(1);

    bool running = true;
    SDL_Event e;

    const int idleWaitMs = 10;

    double monoa = 0;
    bool toggle = true;
    double monob = 0;
    double toggleb = false;

    cam.position = glm::vec3(0.f, 2.f, 0.f);

    // --- Timing variables ---
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 last = 0;
    double deltaTime = 0.0;
    double freq = (double)SDL_GetPerformanceFrequency();

    float virtualX = 400.0f, virtualY = 300.0f; 



    while (running) {
        last = now;
        now = SDL_GetPerformanceCounter();
        deltaTime = (double)(now - last) / freq; // seconds since last frame

        
        // Wait for an event *or* for timeout (so we also animate/render)
        if (SDL_WaitEventTimeout(&e, idleWaitMs)) {
            // We got at least one event — process it
            do {
                if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    if (e.button.button == SDL_BUTTON_RIGHT) {
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
                if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    if (e.button.button == SDL_BUTTON_RIGHT) {
                        monob = 0;
                    }
                }
                if (e.type == SDL_EVENT_MOUSE_MOTION){
                    virtualX += (float)e.motion.xrel;
                    virtualY += (float)e.motion.yrel;
                    cam.Mouse_SetLookAt(virtualX, virtualY, 10);
                   
                }
                if (e.key.key == SDLK_ESCAPE) {
                    monoa += 0.000000001;
                    if (monoa == 0.000000001) {
                        toggle = !toggle;
                        if (toggle == false) {
                            SDL_SetWindowRelativeMouseMode(window, true);  // reports continuous relative motion, hides/locks cursor for that window
                            SDL_HideCursor();
                        }
                        else {
                            // to return to normal:
                            SDL_SetWindowRelativeMouseMode(window, false);
                            SDL_ShowCursor();
                        }
                    }
                    if (e.type == SDL_EVENT_KEY_DOWN) {
                        if (e.key.key == SDLK_ESCAPE) {
                            monoa = 0;
                        }
                    }
                }
                
            } while (SDL_PollEvent(&e));
        }
        const bool* keys = SDL_GetKeyboardState(NULL);
 
        float cameraSpeed = static_cast<float>(5.0f) * deltaTime;
        
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
  
        glClearColor(0.53, 0.81, 0.92, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        
        torus2.rot_z += 0.175;
        torus2.rot_y += 0.12;

        torus1.rot_z += 0.125;
        torus1.rot_y += 0.15;

        
        Engine::Engine::Render(cam);
        

        double fps = 1.0 / deltaTime;

        std::cout << "\rFrame time: " << (deltaTime * 1000.0) << " ms  |  FPS: " << fps << "       " << std::flush;

        
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
