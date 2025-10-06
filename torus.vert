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
	pos.z = lc.z + rt.y * (pos.z - lc.z);

    //change radius
	pos.x += rt.x * cos((theta * lc.y + 90)  * RAD); 
	pos.z += rt.x * sin((theta * lc.y + 90) * RAD);	


    //change rotation
    //z
    pos.x = (cos(40.f * RAD) * pos.x) - (sin(40.f * RAD) * pos.z);
    pos.z = (sin(40.f * RAD) * pos.x) + (cos(40.f * RAD) * pos.z);

    float s = abs(pos.x * pos.x + pos.z * pos.z - 2.0f * rt.x * sqrt(pos.x * pos.x + pos.z * pos.z) + rt.x * rt.x - rt.y * rt.y);

    if (s >= 0){
        pos.y = sqrt(s);
    }



    //change position
    pos += position;

    gl_Position = projection * view * vec4(pos, 1.0f);
}
