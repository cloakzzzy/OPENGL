#version 430 core

layout (location = 0) in vec3 a_VertexPos;
layout (location = 1) in vec3 a_Position;
layout (location = 2) in float a_Radius;
layout (location = 3) in float a_Col;
layout (location = 13) in vec2 a_TexCoord;


layout(std430, binding = 2) buffer GlobalUniforms {
    vec3 CameraPosition;
    mat4 CameraProjection;
    mat4 CameraView;

    uint Num_DirectionalLights;
    uint Num_PointLights;
};


void main()
{
     
    vec3 t_VertexPos = a_VertexPos;

    t_VertexPos *= a_Radius;

    t_VertexPos += a_Position;

   
    gl_Position = CameraProjection * CameraView * vec4(t_VertexPos, 1.0f);

}   
