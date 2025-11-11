#version 330 core
layout (location = 5) in vec3 aPos;
layout (location = 6) in vec3 lc;
 
layout (location = 7) in vec3 position;
layout (location = 8) in vec2 rt;
layout (location = 9) in vec3 Colour;
layout (location = 10) in vec3 rot;

uniform mat4 view;
uniform mat4 projection;
//uniform vec2 change;
uniform float xoff;
uniform float theta;

out vec3 colour;

#define RAD 3.14159f/180.0f

void main()
{
    vec3 pos = aPos;

    //colour = vec3(rt.x,rt.y,0.f);
    colour = Colour;

    //change thickness
    pos.x = lc.x + rt.y * (pos.x - lc.x);
	pos.y = rt.y * pos.y;
	pos.z = lc.z + rt.y * (pos.z - lc.z);

    //change radius
	pos.x += rt.x * cos((theta * lc.y + 90)  * RAD); 
	pos.z += rt.x * sin((theta * lc.y + 90) * RAD);	

    
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

    gl_Position = projection * view * vec4(pos, 1.0f);
}