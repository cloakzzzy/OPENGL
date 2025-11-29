#version 430 core
layout (location = 5) in vec3 a_VertexPos;
 
layout (location = 7) in vec3 a_Position;
layout (location = 8) in vec2 a_RT;
layout (location = 9) in float a_Colour;
layout (location = 10) in vec2 a_Rotation;

uniform mat4 lightSpaceMatrix;


#define RAD 3.14159f/180.0f

layout(std430, binding = 2) buffer CameraData {
    vec3 CameraPosition;
    mat4 CameraProjection;
    mat4 CameraView;

    uint Num_DirectionalLights;
    uint Num_PointLights;
};


void main()
{
    uint u_Col = floatBitsToUint(a_Colour);
    uvec3 Colors = uvec3(u_Col & 0xFFu,  (u_Col >> 8) & 0xFFu, (u_Col >> 16) & 0xFFu); 
    
    vec3 t_VertexPos = a_VertexPos;
   

    vec3 circle_centre = normalize(vec3(t_VertexPos.x, 0.0f, t_VertexPos.z));
    t_VertexPos = (t_VertexPos - circle_centre * 2.0f) + circle_centre * a_RT.x;
    t_VertexPos = circle_centre * a_RT.x + (t_VertexPos - circle_centre * a_RT.x) * a_RT.y;

    vec3 v1 = t_VertexPos;

    
    //pitch
    t_VertexPos.x = (cos(a_Rotation.y * RAD) * v1.x) - (sin(a_Rotation.y * RAD) * v1.y);
    t_VertexPos.y = (sin(a_Rotation.y * RAD) * v1.x) + (cos(a_Rotation.y * RAD) * v1.y);
    vec3 v2 = t_VertexPos;

    //yaw
    t_VertexPos.x = (cos(a_Rotation.x * RAD) * v2.x) - (sin(a_Rotation.x * RAD) * v2.z);
    t_VertexPos.z = (sin(a_Rotation.x * RAD) * v2.x) + (cos(a_Rotation.x * RAD) * v2.z);

    t_VertexPos += a_Position;

    gl_Position = CameraProjection * CameraView * vec4(t_VertexPos, 1.0f);
}