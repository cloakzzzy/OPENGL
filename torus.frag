#version 330 core
out vec4 FragColor;

in vec3 TorusCol;

in vec3 FragPos;
in vec3 BefFrag;
in vec3 TorusPos;
in vec3 Rot;

uniform vec3 ViewPos;
uniform int NumLights;

layout(std140) uniform LightData {
    float values[4096];
};

#define RAD 3.14159f/180.0f

void CalcTorusNormal(out vec3 Normal,vec3 Frag, vec3 Rot){
    vec3 normal = normalize(Frag - normalize(vec3(Frag.x, 0.0f, Frag.z)) * 2.0f);
    vec3 n = normal;
    //pitch
    normal.x = (cos(Rot.z * RAD) * n.x) - (sin(Rot.z * RAD) * n.y);
    normal.y = (sin(Rot.z * RAD) * n.x) + (cos(Rot.z * RAD) * n.y);
    vec3 n1 = normal;
    //yaw
    normal.x = (cos(Rot.y * RAD) * n1.x) - (sin(Rot.y * RAD) * n1.z);
    normal.z = (sin(Rot.y * RAD) * n1.x) + (cos(Rot.y * RAD) * n1.z);

    Normal = normal;
}

void CalcPointLight(out vec3 result, vec3 LightPos, vec3 Terms, vec3 TorusCol, vec3 Normal){
    vec3 LightColour = vec3(1.0f);
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * LightColour;

    //diffuse
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * LightColour;

    // specular
    float specularStrength = 0.4f;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal); 
    
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);

    vec3 specular = specularStrength * spec * LightColour; 

    float distance = length(LightPos - FragPos);
    float attenuation = 1.0 / (Terms.x + Terms.y * distance + Terms.z * (distance * distance));    
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    result = (ambient + diffuse + specular) * TorusCol;
}

void main()
{
    vec3 SumResult = vec3(0.0f);

    vec3 normal;
    CalcTorusNormal(normal, BefFrag, Rot);

    
    for(int i = 0; i < NumLights * 6; i += 6){
        vec3 LightPos = vec3(values[i], values[i + 1], values[i + 2]);
	    vec3 Terms = vec3(values[i + 3], values[i + 4], values[i + 5]);

        vec3 Result;
        CalcPointLight(Result, LightPos, Terms, TorusCol, normal);
        SumResult += Result;
    }

    SumResult = SumResult / (SumResult + vec3(1.0));
  	
    FragColor = vec4(SumResult, 1.0);
}