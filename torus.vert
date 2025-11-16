#version 330 core
layout (location = 5) in vec3 aPos;
//layout (location = 6) in vec3 lc;
 
layout (location = 7) in vec3 position;
layout (location = 8) in vec2 rt;
layout (location = 9) in vec3 Colour;
layout (location = 10) in vec3 rot;

uniform mat4 view;
uniform mat4 projection;
//uniform vec2 change;
uniform float xoff;

out vec3 colour;

#define RAD 3.14159f/180.0f

layout(std140) uniform LightData {
    float values[4096];
};

out vec3 FragPos;
out vec3 TorusPos;
out vec3 BefFrag;
out vec3 TorusCol;
out vec3 Rot;

void main()
{
    //colour = Colour;
    TorusCol = Colour;
    
    vec3 pos = aPos;
    BefFrag = aPos;

    TorusPos = position;
    
    Rot = rot;


   // colour = vec3(values[0]);

    vec3 circle_centre = normalize(vec3(pos.x, 0.0f, pos.z));
    pos = (pos - circle_centre * 2.0f) + circle_centre * rt.x;
    pos = circle_centre * rt.x + (pos - circle_centre * rt.x) * rt.y;

    
    vec3 p = pos;
    //pitch
    pos.x = (cos(rot.z * RAD) * p.x) - (sin(rot.z * RAD) * p.y);
    pos.y = (sin(rot.z * RAD) * p.x) + (cos(rot.z * RAD) * p.y);
    vec3 p1 = pos;
    //yaw
    pos.x = (cos(rot.y * RAD) * p1.x) - (sin(rot.y * RAD) * p1.z);
    pos.z = (sin(rot.y * RAD) * p1.x) + (cos(rot.y * RAD) * p1.z);
    
    

    //change position
    pos += position;

    FragPos = pos;

    gl_Position = projection * view * vec4(pos, 1.0f);
}