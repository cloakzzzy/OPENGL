#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 position;
layout (location = 2) in float radius;
layout (location = 3) in vec3 aCol;

out vec3 pos;
out vec3 col;

uniform mat4 projection;
uniform mat4 view;

#define RAD 3.14159f/180.0f


void main()
{
    col = aCol;
    pos = aPos;

    pos *= radius;

    //pos.x = (cos(100.f * RAD) * pos.x) - (sin(100.f * RAD) * pos.z);
    //pos.z = (sin(100.f * RAD) * pos.x) + (cos(100.f * RAD) * pos.z);

    pos += position;
    
    gl_Position = projection * view * vec4(pos, 1.0f);
      
}
