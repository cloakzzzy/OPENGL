#version 330 core
out vec4 FragColor;

in vec3 SphereCol;

in vec3 FragPos;
in vec3 SpherePos;

uniform vec3 ViewPos;
uniform int NumLights;

layout(std140) uniform LightData {
    float values[4096];
};

void CalcSphereNormal(out vec3 Normal,vec3 FragPos){
    Normal = normalize(FragPos - SpherePos);
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
    float specularStrength = 0.6f;
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
	

    if(SphereCol.x == 1.0f && SphereCol.y == 1.0f && SphereCol.z == 1.0f){
        FragColor = vec4(1.0f);
        return;
    }


    vec3 SumResult = vec3(0.0f);

    vec3 normal;
    CalcSphereNormal(normal, FragPos);
    
    for(int i = 0; i < NumLights * 6; i += 6){
        vec3 LightPos = vec3(values[i], values[i + 1], values[i + 2]);
	    vec3 LightColour = vec3(values[i + 3], values[i + 4], values[i + 5]);

        vec3 Result;
        CalcPointLight(Result, LightPos, LightColour, SphereCol, normal);
        SumResult += Result;
    }

    SumResult = SumResult / (SumResult + vec3(1.0));
  	
    FragColor = vec4(SumResult, 1.0);
}