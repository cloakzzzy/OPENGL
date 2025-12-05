#pragma once
#include "libs/glm/glm.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtc/type_ptr.hpp"
#include <string>

using namespace std;

constexpr float DEG = 180 / 3.14159;

namespace Engine {
    namespace Entity {
        class Camera;
    }
}
class Engine::Entity::Camera
{
private:
    bool firstMouse = true;
    float yaw = -90.0f;
    float pitch = 0.0f;
    float lastX;
    float lastY;
    float nea;
    float fa;

public:
    float fov = 45.0f;
    float aspect;
    glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 position = glm::vec3(3.0f, 3.0f, 3.0f);
    glm::vec2 yawdir = glm::vec2(0.0f, -1.0);

    Camera(float windowwidth, float windowheight, float Fov, float Aspect, float Near, float Far);

    void Mouse_SetLookAt(float xposIn, float yposIn, float sensitivity);

    void Controller_HandleRightStick(float SensitivityX, float SensitivityY, float Xdegrees, float Ydegrees, float deadzone);

    void Controller_HandleLeftStick(float Xdegrees, float Ydegrees, float speed, float deadzone, float dt);

    glm::mat4 GetView() {
        glm::mat4 view = glm::lookAt(position, position + direction, up);
        return view;
    }

    glm::mat4 GetProjection() {
        glm::mat4 projection = glm::perspective(glm::radians(fov), aspect, nea, fa);
        return projection;
    }
};