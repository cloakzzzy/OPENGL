#version 330 core
out vec4 FragColor;

in vec3 TorusCol;

in vec3 FragPos;
in vec3 BefFrag;
in vec3 TorusPos;
in vec3 Rot;

uniform vec3 ViewPos;

layout(std140) uniform LightData {
    float values[4096];
};

#define RAD 3.14159f/180.0f

void main()
{
	vec3 LightPos = vec3(values[0], values[1], values[2]);
	vec3 LightColour = vec3(values[3], values[4], values[5]);


    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * LightColour;
  	
    // diffuse 
    vec3 norm = normalize(BefFrag - normalize(vec3(BefFrag.x, 0.0f, BefFrag.z)) * 2.0f);

    vec3 n = norm;
    //pitch
    norm.x = (cos(Rot.z * RAD) * n.x) - (sin(Rot.z * RAD) * n.y);
    norm.y = (sin(Rot.z * RAD) * n.x) + (cos(Rot.z * RAD) * n.y);
    vec3 n1 = norm;
    //yaw
    norm.x = (cos(Rot.y * RAD) * n1.x) - (sin(Rot.y * RAD) * n1.z);
    norm.z = (sin(Rot.y * RAD) * n1.x) + (cos(Rot.y * RAD) * n1.z);

    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * LightColour;
    
    // specular
    float specularStrength = 0.4f;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * LightColour;  
        
    vec3 result = (ambient + diffuse + specular) * TorusCol;
    FragColor = vec4(result, 1.0);
}