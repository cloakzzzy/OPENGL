#version 430 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 position;
layout (location = 2) in float radius;
layout (location = 3) in vec3 aCol;
layout (location = 13) in vec2 aTexCoord;


uniform mat4 projection;
uniform mat4 view;

out vec3 FragPos;
out vec3 SpherePos;
out vec3 SphereCol;

out vec2 TexCoord;

void main()
{
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    SpherePos = position;
    SphereCol = aCol;
    
    vec3 pos = aPos;

    pos *= radius;

    pos += position;

    FragPos = pos;
    
    gl_Position = projection * view * vec4(pos, 1.0f);
}   

