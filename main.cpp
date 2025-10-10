
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
    vector<float> crosshair;

    Crosshair(crosshair);


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

    vector<float> spheres(7 * 18);

    TorusBuffer tb;
    float* torus_buffer = tb.Create(10 * 11, 70);

    
    torus_buffer[0] = 0.f;
    torus_buffer[1] = 0.f;
    torus_buffer[2] = 0.f;

    torus_buffer[3] = 2.5f;
    torus_buffer[4] = 1.3f;

    torus_buffer[5] = 0.f;
    torus_buffer[6] = 0.5f;
    torus_buffer[7] = 0.0f;

    torus_buffer[8] = 0.0f;
    torus_buffer[9] = 0.0f;
    torus_buffer[10] = 0.f;
    

    torus_buffer[11] = 3.f;
    torus_buffer[12] = 2.f;
    torus_buffer[13] = 1.f;

    torus_buffer[14] = 2.5f;
    torus_buffer[15] = 1.3f;

    torus_buffer[16] = 1.f;
    torus_buffer[17] = 0.f;
    torus_buffer[18] = 0.0f;

    torus_buffer[19] = 0.0f;
    torus_buffer[20] = 113.f;
    torus_buffer[21] = 64.f;



    tb.SendToGPU(0, 11);

    float R = 2.5f;
    float T = 1.3f;



    processInput(win.Object);

    VAO1.Bind();


    float ang = 0.f;
    const unsigned int h = 50;



    win.MainLoop([&] {

        processInput(win.Object);


        tb.SendToGPU(0, 22);

        glm::vec4 roots;

        std::fill(spheres.begin(), spheres.end(), 0.0f);

        glm::vec3 r = glm::normalize(glm::vec3(cam.direction.x, cam.direction.y, cam.direction.z));
        glm::vec3 p = cam.position - glm::vec3(3.0f, 2.0f, 1.0f);

        
        glm::vec3 p2 = p;
        p.x = p2.x * cos(-torus_buffer[20] * RAD) - p2.z * sin(-torus_buffer[20] * RAD);
        p.z = p2.x * sin(-torus_buffer[20] * RAD) + p2.z * cos(-torus_buffer[20] * RAD);
        glm::vec3 p1 = p;
        p.x = p1.x * cos(-torus_buffer[21] * RAD) - p1.y * sin(-torus_buffer[21] * RAD);
        p.y = p1.x * sin(-torus_buffer[21] * RAD) + p1.y * cos(-torus_buffer[21] * RAD);

        glm::vec3 r2 = r;
        r.x = r2.x * cos(-torus_buffer[20] * RAD) - r2.z * sin(-torus_buffer[20] * RAD);
        r.z = r2.x * sin(-torus_buffer[20] * RAD) + r2.z * cos(-torus_buffer[20] * RAD);

        glm::vec3 r1 = r;
        r.x = r1.x * cos(-torus_buffer[21] * RAD) - r1.y * sin(-torus_buffer[21] * RAD);
        r.y = r1.x * sin(-torus_buffer[21] * RAD) + r1.y * cos(-torus_buffer[21] * RAD);
        
        //shows ghost ray, with spheres
        
        float t = 4.f;
        for (int i = 0; i < 91; i+=7) {
            spheres[i] = p.x + r.x * t;
            spheres[i + 1] = p.y + r.y * t;
            spheres[i + 2] = p.z + r.z * t;
            spheres[i + 3] = 0.1f;
            spheres[i + 4] = 1.f;
            spheres[i + 5] = 1.f;
            spheres[i + 6] = 0.f;
            t += 3.f;
        }
       

        //green,cyan, blue
        //cyan, green ,blue

       // std::cout << r.x << " " << r.y << " " << r.z << '\n';


        /*
        spheres[7] = r.x;
        spheres[8] = r.y;
        spheres[9] = r.z;
        spheres[10] = 0.1f;
        spheres[11] = 0.0f;
        spheres[12] = 0.f;
        spheres[13] = 1.f;



        spheres[0] = r2.x;
        spheres[1] = r2.y;
        spheres[2] = r2.z;
        spheres[3] = 0.1f;
        spheres[4] = 0.0f;
        spheres[5] = 1.f;
        spheres[6] = 0.f;
        */
       // torus_buffer[20] += 0.5f;
        //torus_buffer[21] += 0.5f;



        float e = 2.0f * p.x * r.x + 2.0f * p.z * r.z;
        float f = p.x * p.x + p.z * p.z;
        float d = r.x * r.x + r.z * r.z;
        float c = -(p.x * p.x + p.y * p.y + p.z * p.z) + (T * T - R * R);
        float b = -(2.0f * p.x * r.x + 2.0f * p.y * r.y + 2.0f * p.z * r.z);
        float a = -(r.x * r.x + r.y * r.y + r.z * r.z);
        float A = a * a;
        float B = 2.0f * a * b;
        float C = 2.0f * a * c + b * b - 4.0f * R * R * d;
        float D = 2.0f * b * c - 4.0f * R * R * e;
        float E = c * c - 4.0f * R * R * f;

        int nroots = solveQuartic(A, B, C, D, E, roots);
        

        std::cout << roots.x << '\n';
        std::cout << roots.y << '\n';
        std::cout << roots.z << '\n';
        std::cout << roots.w << '\n';

        std::cout << "=========================\n";
        
        
        
        if (nroots > 0) {
            glm::vec3 h = p + r * roots[0];

            
            glm::vec3 h1 = h;
            h.x = h1.x * cos(torus_buffer[21] * RAD) - h1.y * sin(torus_buffer[21] * RAD);
            h.y = h1.x * sin(torus_buffer[21] * RAD) + h1.y * cos(torus_buffer[21] * RAD);
            glm::vec3 h2 = h;
            h.x = h2.x * cos(torus_buffer[20] * RAD) - h2.z * sin(torus_buffer[20] * RAD);
            h.z = h2.x * sin(torus_buffer[20] * RAD) + h2.z * cos(torus_buffer[20] * RAD);

            h += glm::vec3(3.0f, 2.0f, 1.0f);
            

            spheres[98] = h.x;
            spheres[99] = h.y;
            spheres[100] = h.z;
            spheres[101] = 0.15f;
        }

        if (nroots > 1) {
            glm::vec3 h = p + r * roots[1];
            
            
            glm::vec3 h1 = h;
            h.x = h1.x * cos(torus_buffer[21] * RAD) - h1.y * sin(torus_buffer[21] * RAD);
            h.y = h1.x * sin(torus_buffer[21] * RAD) + h1.y * cos(torus_buffer[21] * RAD);
            glm::vec3 h2 = h;
            h.x = h2.x * cos(torus_buffer[20] * RAD) - h2.z * sin(torus_buffer[20] * RAD);
            h.z = h2.x * sin(torus_buffer[20] * RAD) + h2.z * cos(torus_buffer[20] * RAD);

            h += glm::vec3(3.0f, 2.0f, 1.0f);
            

            spheres[105] = h.x;
            spheres[106] = h.y;
            spheres[107] = h.z;
            spheres[108] = 0.15f;
        }
        if (nroots > 2) {
            glm::vec3 h = p + r * roots[2];
            
            
            glm::vec3 h1 = h;
            h.x = h1.x * cos(torus_buffer[21] * RAD) - h1.y * sin(torus_buffer[21] * RAD);
            h.y = h1.x * sin(torus_buffer[21] * RAD) + h1.y * cos(torus_buffer[21] * RAD);
            glm::vec3 h2 = h;
            h.x = h2.x * cos(torus_buffer[20] * RAD) - h2.z * sin(torus_buffer[20] * RAD);
            h.z = h2.x * sin(torus_buffer[20] * RAD) + h2.z * cos(torus_buffer[20] * RAD);

            h += glm::vec3(3.0f, 2.0f, 1.0f);
            

            spheres[112] = h.x;
            spheres[113] = h.y;
            spheres[114] = h.z;
            spheres[115] = 0.15f;
        }

        if (nroots > 3) {
            glm::vec3 h = p + r * roots[3];
            
            
            glm::vec3 h1 = h;
            h.x = h1.x * cos(torus_buffer[21] * RAD) - h1.y * sin(torus_buffer[21] * RAD);
            h.y = h1.x * sin(torus_buffer[21] * RAD) + h1.y * cos(torus_buffer[21] * RAD);
            glm::vec3 h2 = h;
            h.x = h2.x * cos(torus_buffer[20] * RAD) - h2.z * sin(torus_buffer[20] * RAD);
            h.z = h2.x * sin(torus_buffer[20] * RAD) + h2.z * cos(torus_buffer[20] * RAD);

            h += glm::vec3(3.0f, 2.0f, 1.0f);
            

            spheres[119] = h.x;
            spheres[120] = h.y;
            spheres[121] = h.z;
            spheres[122] = 0.15f;
        }
        

        VAO1.Bind();

        glBindBuffer(GL_ARRAY_BUFFER, sphere_instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0.f, spheres.size() * sizeof(float), &spheres.front());

        sphere_vbo.Bind();
        sphere_shader.Use();
        sphere_shader.SetMat4("projection", glm::value_ptr(cam.GetProjection()));
        sphere_shader.SetMat4("view", glm::value_ptr(cam.GetView()));


        glDrawArraysInstanced(GL_TRIANGLES, 0, vertices.size() / 3.0f, 18);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        VAO1.Unbind();
        tb.Render(cam, 2);




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