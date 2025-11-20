#version 330 core
layout (location = 11) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out vec3 FragPos;
out vec4 FragPosLightSpace;

void main()
{
    FragPos = aPos;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    

    gl_Position = projection * view * vec4(aPos, 1.0f);
}