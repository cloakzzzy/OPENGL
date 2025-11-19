#version 430 core
out vec4 FragColor;

in vec3 SphereCol;

in vec3 FragPos;
in vec3 SpherePos;
in vec2 TexCoord;



layout(std430, binding = 2) buffer GlobalUniforms {
    vec3 CameraPosition;
    mat4 CameraProjection;
    mat4 CameraView;
    
    uint Num_DirectionalLights;
    uint Num_PointLights;
};


layout(std430, binding = 0) buffer Data_PointLight {
    float PointLight_Values[];
};

layout(std430, binding = 1) buffer Data_DirectionalLight {
    float DirectionalLight_Values[];
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
    vec3 viewDir = normalize(CameraPosition - FragPos);
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

void CalcDirectionalLight(out vec3 result, vec3 lightDir, vec3 TorusCol, vec3 Normal){
    vec3 LightColour = vec3(1.0f);
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * LightColour;

    //diffuse
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * LightColour;

    // specular
    float specularStrength = 0.6f;
    vec3 viewDir = normalize(CameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);

    vec3 specular = specularStrength * spec * LightColour; 


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

    for(int i = 0; i < Num_PointLights * 6; i += 6){
        vec3 LightPos = vec3(PointLight_Values[i], PointLight_Values[i + 1], PointLight_Values[i + 2]);
	    vec3 Terms = vec3(PointLight_Values[i + 3], PointLight_Values[i + 4], PointLight_Values[i + 5]);

        vec3 Result;
        CalcPointLight(Result, LightPos, Terms, SphereCol , normal);
        SumResult += Result;
    }

    for(int i = 0; i < Num_DirectionalLights * 3; i+=3){
        vec3 LightDir = normalize(vec3(DirectionalLight_Values[i], DirectionalLight_Values[i + 1], DirectionalLight_Values[i + 2]));
	    
        vec3 Result;
        CalcDirectionalLight(Result, LightDir, SphereCol, normal);
        SumResult += Result;
    }



    SumResult = SumResult / (SumResult + vec3(1.0));
  	
    FragColor = vec4(SumResult, 1.0);
}