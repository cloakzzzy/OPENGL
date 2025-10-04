
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

#include <cstdarg>
#include <iostream>
#include <vector>
#include <thread>
#include <cstring>

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
   

    vector <float> vertices;
   
    Gen_UVsphere(vertices, 50, 0.0f, 0.f, 0.f, 1.0f);

    VAO VAO1;

    VAO1.Bind();

    Shader sphere_shader;
    sphere_shader.SetFiles("sphere.vert", "sphere.frag");
    sphere_shader.Use();

    VBO sphere_vbo;
    sphere_vbo.Create(vertices);

    VAO1.LinkVBO(sphere_vbo, 6, 0, 3, 0);

    unsigned int sphere_instanceVBO;

    glGenBuffers(1, &sphere_instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, 7 * 20 * sizeof(float), __nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, sphere_instanceVBO);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
    glVertexAttribDivisor(3, 1);

    VAO1.Unbind();

    vector<float> spheres(28);

    TorusBuffer tb;
    float* torus_buffer = tb.Create(10 * 11, 70);

    torus_buffer[0] = 5.0f;
    torus_buffer[1] = 20.f;
    torus_buffer[2] = 15.0f;

    torus_buffer[3] = 2.0f;
    torus_buffer[4] = 1.0f;

    torus_buffer[5] = 0.f;
    torus_buffer[6] = 0.5f;
    torus_buffer[7] = 0.0f;

    torus_buffer[8] = 0.0f;
    torus_buffer[9] = 0.0f;
    torus_buffer[10] = 0.0f;

    tb.SendToGPU(0, 11);

    float T = 1.0f;
    float R = 2.0f;

    
    processInput(win.Object);

    cam.position.z += 3.0f;
    VAO1.Bind();


    const unsigned int h = 50;

    win.MainLoop([&] {

        processInput(win.Object);
        
       
        glm::vec3 r = glm::normalize(glm::vec3(cam.direction.x, cam.direction.y, cam.direction.z));

        float px = cam.position.x - 5.f;
        float py = cam.position.y - 20.f;
        float pz = cam.position.z - 15.f;


        float e = 2.0f * px * r.x + 2.0f * pz * r.z;
        float f = px * px + pz * pz;
        float d = r.x * r.x + r.z * r.z;
        float c = -(px * px + py * py + pz * pz) + (T * T - R * R);
        float b = -(2.0f * px * r.x + 2.0f * py * r.y + 2.0f * pz * r.z);
        float a = -(r.x * r.x + r.y * r.y + r.z * r.z);
        float A = a * a;
        float B = 2.0f * a * b;
        float C = 2.0f * a * c + b * b - 4.0f * R * R * d;
        float D = 2.0f * b * c - 4.0f * R * R * e;
        float E = c * c - 4.0f * R * R * f;


        glm::vec4 roots;

        //int nroots;
        int nroots = solveQuartic(A, B, C, D, E, roots);
        //int nroots = solveQuartic(1, -7, 17, -17, 6, roots);

        
        std::cout << nroots << ":\n";
        std::cout << roots.x << '\n';
        std::cout << roots.y << '\n';
        std::cout << roots.z << '\n';
        std::cout << roots.w << '\n';
        
        std::cout << "-------------------" << '\n';

        std::fill(spheres.begin(), spheres.end(), 0.0f);
        if (nroots > 0) {
            spheres[0] = cam.position.x + r.x * roots[0];
            spheres[1] = cam.position.y + r.y * roots[0];
            spheres[2] = cam.position.z + r.z * roots[0];
            spheres[3] = 0.1f;
        }

        if (nroots > 1) {
            spheres[7] = cam.position.x + r.x * roots[1];
            spheres[8] = cam.position.y + r.y * roots[1];
            spheres[9] = cam.position.z + r.z * roots[1];
            spheres[10] = 0.1f;
        }
        if (nroots > 2) {
            spheres[14] = cam.position.x + r.x * roots[2];
            spheres[15] = cam.position.y + r.y * roots[2];
            spheres[16] = cam.position.z + r.z * roots[2];
            spheres[17] = 0.1f;
        }
        if (nroots > 3) {
            spheres[21] = cam.position.x + r.x * roots[3];
            spheres[22] = cam.position.y + r.y * roots[3];
            spheres[23] = cam.position.z + r.z * roots[3];
            spheres[24] = 0.1f;
        }


        //for (int i = 0; i < 7; i++) {
            //std::cout << spheres[i] << '\n';

        //}

        std::cout << "\n=========================\n";

        VAO1.Bind();

        glBindBuffer(GL_ARRAY_BUFFER, sphere_instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0.f, spheres.size() * sizeof(float), &spheres.front());

        sphere_vbo.Bind();
        sphere_shader.Use();
        sphere_shader.SetMat4("projection", glm::value_ptr(cam.GetProjection()));
        sphere_shader.SetMat4("view", glm::value_ptr(cam.GetView()));


        glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size() / 3.0f, floor(spheres.size() / 7.0f));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        VAO1.Unbind();
        tb.Render(cam, 1);




        }, 0.53, 0.81, 0.92);


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