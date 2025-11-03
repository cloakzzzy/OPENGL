
#include <GL/glew.h>
#include <glfw3.h>
#include <math.h>

#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"

#include "Macros.hpp"
#include "OtherAbstractions.hpp"
#include "Depthshapes.hpp"

#include "VBO.hpp"
#include "VAO.hpp"
#include "EBO.hpp"

#include "Shader.hpp"
#include "Camera.hpp"
#include "Torus.hpp"
#include <chrono>

#include <cstdarg>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>
#include <iomanip>

#include "Engine.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window);

const unsigned long SCR_WIDTH = 800;
const unsigned long SCR_HEIGHT = 600;

float s = 0.01f;

void Crosshair(vector<float>& vertices) {
    Gen_Quad(vertices, -0.005, 0.005, s, s);
    Gen_Quad(vertices, 0.005, 0.005, s, s);
    Gen_Quad(vertices, -0.005, -0.005, s, s);
    Gen_Quad(vertices, -0.015, 0.005, s, s);
    Gen_Quad(vertices, -0.005, 0.015, s, s);
}

unsigned int Frames = 0;
Camera cam(SCR_WIDTH, SCR_HEIGHT, 90, float(SCR_WIDTH) / float(SCR_HEIGHT), 0.1, 100);
Window win;
const unsigned char* buttons;
//Shader shader;

int solveQuartic(float a, float b, float c, float d, float e, glm::vec4& roots) {
    b /= a; c /= a; d /= a; e /= a;
    float bb = b * b; float p = (8.0 * c - 3.0 * bb) / 8.0; float q = (8.0 * d - 4.0 * c * b + bb * b) / 8.0;
    float r = (256.0 * e - 64.0 * d * b + 16.0 * c * bb - 3.0 * bb * bb) / 256.0; int n = 0; float ra = 2.0 * p; float rb = p * p - 4.0 * r;
    float rc = -q * q; float ru = ra / 3.0; float rp = rb - ra * ru; float rq = rc - (rb - 2.0 * ra * ra / 9.0) * ru; float lambda;
    float rh = 0.25 * rq * rq + rp * rp * rp / 27.0;
    if (rh > 0.0) { rh = sqrt(rh); float ro = -0.5 * rq; lambda = cbrt(ro - rh) + cbrt(ro + rh) - ru; }
    else { float rm = sqrt(-rp / 3.0); lambda = -2.0 * rm * sin(asin(1.5 * rq / (rp * rm)) / 3.0) - ru; }
    for (int i = 0; i < 2; i++) {
        float a_2 = ra + lambda; float a_1 = rb + lambda * a_2; float b_2 = a_2 + lambda;
        float f = rc + lambda * a_1; float f1 = a_1 + lambda * b_2; lambda -= f / f1;
    }
    if (lambda < 0.0) return n;
    float t = sqrt(lambda); float alpha = 2.0 * q / t, beta = lambda + ra; float u = 0.25 * b; t *= 0.5; float z = -alpha - beta;

    if (z > 0.0) { z = sqrt(z) * 0.5; float h = +t - u; roots.x = h + z; roots.y = h - z; n += 2; }
    float w = +alpha - beta;
    if (w > 0.0) {
        w = sqrt(w) * 0.5; float h = -t - u; roots.z = h + w; roots.w = h - w;
        if (n == 0) { roots.x = roots.z; roots.y = roots.w; }
        n += 2;
    }
    return n;
}

using Engine::Entity::Primitive;


int main()
{
    glfwSetVersion(3, 3);
    win.Create(SCR_WIDTH, SCR_HEIGHT, "hello");

    glfwSwapInterval(0);
    glfwSetFramebufferSizeCallback(win.Object, framebuffer_size_callback);
    glfwSetCursorPosCallback(win.Object, mouse_callback);
    glfwSetMouseButtonCallback(win.Object, mouse_button_callback);
    glfwSetInputMode(win.Object, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    glewInit();
    glEnable(GL_DEPTH_TEST);


    Engine::Engine::Initialize();
    auto torus1 = Primitive::CreateTorus(0.0f, 0.0f, 0.0f,
        1.f, 1.f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f);

    auto torus2 = Primitive::CreateTorus(0.0f, 0.0f, 5.0f,
        2.f, 1.f,
        0.0f, 0.0f, 1.f,
        0.0f, 0.0f, 0.0f);

    // 1,2,3,4,5

    //torus5.rot_x += 240.f;

    //torus3.Delete();
   // torus4.Delete();

    float x = 2.0f;
    float y = 3.0f;

    /*
    auto start = std::chrono::high_resolution_clock::now();

    for (long long i = 0; i < 1000000; i++) {
        torus5.rot_x += torus2.pos_z * 3.0f;
       // x += y * 3.0f;

    }


    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time taken: " << duration << " ms\n";

    std::cout << x << '\n';
    */

    std::cout << "Array:\n\n";



    for (int i = 0; i < Torus::InstanceBuffer.size(); i++) {
        if (i % 11 == 0) {
            std::cout << "==================\n";
        }
        std::cout << std::setprecision(10) << Torus::InstanceBuffer[i] << '\n';
    }





    float ang = 0.f;
    const unsigned int h = 50;


    cam.position = glm::vec3(0.f, 0.f, 0.f);
    while (!glfwWindowShouldClose(win.Object))
    {
        Sleep(1);
        processInput(win.Object);
        s = round(glfwGetTime());
        Frames += 1;
        float currentFrame = static_cast<float>(glfwGetTime());
        win.DeltaTime = currentFrame - win.LastFrame;
        win.LastFrame = currentFrame;

        glClearColor(0.53, 0.81, 0.92, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        torus2.rot_z += 0.175 * (1 + win.DeltaTime);
        torus2.rot_y += 0.12 * (1 + win.DeltaTime);

        torus1.rot_z += 0.125 * (1 + win.DeltaTime);
        torus1.rot_y += 0.15 * (1 + win.DeltaTime);
        
        Engine::Engine::Render(cam);

        if (win.DeltaTime * 1000 > 16.6) {
            std::cout << "[BELOW 60FPS]" << '\n';
        }

        glfwSwapBuffers(win.Object);
        glfwPollEvents();

        //std::cout << win.DeltaTime * 1000 << '\n';
    
    }

    glfwTerminate();
    return 0;
}

double monoa = 0;
bool toggle = true;

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        monoa += 0.000000001;
        if (monoa == 0.000000001) {
            toggle = !toggle;
            if (toggle == false) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
        monoa = 0;

    float cameraSpeed = static_cast<float>(5.0f) * win.DeltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam.position.x += cameraSpeed * cam.yawdir.x;
        cam.position.z += cameraSpeed * cam.yawdir.y;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam.position.x -= cameraSpeed * cam.yawdir.x;
        cam.position.z -= cameraSpeed * cam.yawdir.y;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.position -= glm::normalize(glm::cross(cam.direction, cam.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.position += glm::normalize(glm::cross(cam.direction, cam.up)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cam.position.y += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cam.position.y -= cameraSpeed;


}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    cam.aspect = float(width) / float(height);

}
double sxpos;
double sypos;
bool set = true;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (toggle == true) {
        cam.Mouse_SetLookAt(xposIn, yposIn, 10);
    }
}

double monob = 0;
bool toggleb = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
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
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        monob = 0;


}