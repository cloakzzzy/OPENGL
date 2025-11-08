#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 position;
layout (location = 2) in float radius;
layout (location = 3) in vec3 aCol;

out vec3 col;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    col = aCol;

    vec3 pos = aPos;

    pos *= radius;

    pos += position;
    
    gl_Position = projection * view * vec4(pos, 1.0f);
}   

