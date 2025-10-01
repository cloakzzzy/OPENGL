#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 5) in vec3 lc;
 
layout (location = 1) in vec3 position;
layout (location = 2) in vec2 rt;
layout (location = 3) in vec3 Colour;
layout (location = 4) in vec3 rot;

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

    colour = Colour;

    //change thickness
    pos.x = lc.x + rt.y * (pos.x - lc.x);
	pos.y = rt.y * pos.y;
	pos.z = lc.z + rt.y * (pos.z - lc.z);

    //change radius
	pos.x += rt.x * cos((theta * lc.y + 90)  * RAD); 
	pos.z += rt.x * sin((theta * lc.y + 90) * RAD);	


    //change rotation
    //z
    pos.x = (cos(rot.z * RAD) * pos.x) - (sin(rot.z * RAD) * pos.y);
    pos.y = (sin(rot.z * RAD) * pos.x) + (cos(rot.z * RAD) * pos.y);
    //change position
    pos += position;

    gl_Position = projection * view * vec4(pos, 1.0f);
}
