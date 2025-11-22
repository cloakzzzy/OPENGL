#version 430 core
layout (location = 11) in vec3 aPos;

layout(std430, binding = 2) buffer GlobalUniforms {
    vec3 CameraPosition;
    mat4 CameraProjection;
    mat4 CameraView;

    uint Num_DirectionalLights;
    uint Num_PointLights;
};

void main()
{
    gl_Position = CameraProjection * CameraView * vec4(aPos, 1.0f);
}