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

out vec4 FragPosLightSpace[3];
uniform mat4 lightSpaceMatrix[3];

out vec3 FragPos;
out vec3 SpherePos;
out vec3 SphereCol;

out vec2 TexCoord;

void main()
{
    uint u_Col = floatBitsToUint(a_Col);
    uvec3 Colors = uvec3(u_Col & 0xFFu,  (u_Col >> 8) & 0xFFu, (u_Col >> 16) & 0xFFu); 
    SphereCol = vec3(Colors / 255.0f);
    
    TexCoord = vec2(a_TexCoord.x, a_TexCoord.y);
    SpherePos = a_Position;
    
    
    vec3 t_VertexPos = a_VertexPos;

    t_VertexPos *= a_Radius;

    t_VertexPos += a_Position;

    FragPos = t_VertexPos;

    for (int i = 0;i < 3; i++){
        FragPosLightSpace[i] = lightSpaceMatrix[i] * vec4(FragPos, 1.0);
    }
    
    gl_Position = CameraProjection * CameraView * vec4(t_VertexPos, 1.0f);

}   
