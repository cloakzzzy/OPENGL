#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 position;
layout (location = 2) in float radius;
layout (location = 3) in vec3 aCol;

layout(std140) uniform LightData {
    float values[4096];
};



uniform mat4 projection;
uniform mat4 view;

out vec3 FragPos;
out vec3 SpherePos;
out vec3 SphereCol;

void main()
{
    
    SpherePos = position;
    SphereCol = aCol;
    
    vec3 pos = aPos;

    pos *= radius;

    pos += position;

    FragPos = pos;
    
    gl_Position = projection * view * vec4(pos, 1.0f);
}   

