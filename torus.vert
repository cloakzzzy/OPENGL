#version 430 core
layout (location = 5) in vec3 aVertex;
 
layout (location = 7) in vec3 aPosition;
layout (location = 8) in vec2 aRT;
layout (location = 9) in vec3 aColour;
layout (location = 10) in vec3 aRotation;

uniform mat4 view;
uniform mat4 projection;
//uniform vec2 change;
uniform float xoff;

out vec3 colour;

#define RAD 3.14159f/180.0f

out vec3 FragPos;
out vec3 TorusPos;
out vec3 BefFrag;
out vec3 TorusCol;
out vec3 Rotation;

void main()
{
    TorusCol = aColour;
    
    vec3 Vertex = aVertex;
    BefFrag = aVertex;

    TorusPos = aPosition;
    
    Rotation = aRotation;


   // colour = vec3(values[0]);

    vec3 circle_centre = normalize(vec3(Vertex.x, 0.0f, Vertex.z));
    Vertex = (Vertex - circle_centre * 2.0f) + circle_centre * aRT.x;
    Vertex = circle_centre * aRT.x + (Vertex - circle_centre * aRT.x) * aRT.y;

    
    vec3 v1 = Vertex;
    //pitch
    Vertex.x = (cos(aRotation.z * RAD) * v1.x) - (sin(aRotation.z * RAD) * v1.y);
    Vertex.y = (sin(aRotation.z * RAD) * v1.x) + (cos(aRotation.z * RAD) * v1.y);
    vec3 v2 = Vertex;
    //yaw
    Vertex.x = (cos(aRotation.y * RAD) * v2.x) - (sin(aRotation.y * RAD) * v2.z);
    Vertex.z = (sin(aRotation.y * RAD) * v2.x) + (cos(aRotation.y * RAD) * v2.z);

    //change position
    Vertex += aPosition;

    FragPos = Vertex;

    gl_Position = projection * view * vec4(Vertex, 1.0f);
}