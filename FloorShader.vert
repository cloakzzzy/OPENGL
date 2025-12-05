#version 430 core
layout (location = 11) in vec3 aPos;

layout(std430, binding = 2) buffer GlobalUniforms {
    vec3 CameraPosition;
    mat4 CameraProjection;
    mat4 CameraView;

    uint Num_DirectionalLights;
    uint Num_PointLights;
};

uniform mat4 lightSpaceMatrix[3];

out vec3 FragPos;
out vec4 FragPosLightSpace[3];


void main()
{
    FragPos = aPos;
    

    for (int i = 0;i < Num_DirectionalLights; i++){
        FragPosLightSpace[i] = lightSpaceMatrix[i] * vec4(FragPos, 1.0);
    }

    gl_Position = CameraProjection * CameraView * vec4(aPos, 1.0f);
}