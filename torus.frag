#version 430 core
out vec4 FragColor;

in vec3 TorusCol;

in vec3 FragPos;
in vec3 BefFrag;
in vec3 TorusPos;
in vec3 Rotation;

uniform vec3 ViewPos;
uniform int Num_PointLights;
uniform int Num_DirectionalLights;

// SSBO at binding = 0

// Struct definition

layout(std430, binding = 0) buffer LightData {
    float PointLight_Values[];
};

// SSBO at binding = 0
layout(std430, binding = 1) buffer Data_DirectionalLight {
    float DirectionalLight_Values[];
};

#define RAD 3.14159f/180.0f

void CalcTorusNormal(out vec3 Normal,vec3 Frag, vec3 Rotation){
    vec3 normal = normalize(Frag - normalize(vec3(Frag.x, 0.0f, Frag.z)) * 2.0f);
    vec3 n = normal;
    //pitch
    normal.x = (cos(Rotation.z * RAD) * n.x) - (sin(Rotation.z * RAD) * n.y);
    normal.y = (sin(Rotation.z * RAD) * n.x) + (cos(Rotation.z * RAD) * n.y);
    vec3 n1 = normal;
    //yaw
    normal.x = (cos(Rotation.y * RAD) * n1.x) - (sin(Rotation.y * RAD) * n1.z);
    normal.z = (sin(Rotation.y * RAD) * n1.x) + (cos(Rotation.y * RAD) * n1.z);

    Normal = normal;
}

void CalcPointLight(out vec3 result, vec3 LightPos, vec3 Terms, vec3 TorusCol, vec3 Normal){
    vec3 LightColour = vec3(1.0f);
    // ambient
    float AmbientStrength = 0.1;
    vec3 Ambient = AmbientStrength * LightColour;

    //diffuse
    vec3 LightDir = normalize(LightPos - FragPos);
    float Diff = max(dot(Normal, LightDir), 0.0);
    vec3 Diffuse = Diff * LightColour;

    // specular
    float SpecularStrength = 0.4f;
    vec3 ViewDir = normalize(ViewPos - FragPos);
    vec3 ReflectDir = reflect(-LightDir, Normal); 
    
    vec3 HalfwayDir = normalize(LightDir + ViewDir);

    float Spec = pow(max(dot(Normal, HalfwayDir), 0.0), 16.0);

    vec3 Specular = SpecularStrength * Spec * LightColour; 

    float Distance = length(LightPos - FragPos);
    float Attenuation = 1.0 / (Terms.x + Terms.y * Distance + Terms.z * (Distance * Distance));    
    
    Ambient *= Attenuation;
    Diffuse *= Attenuation;
    Specular *= Attenuation;

    result = (Ambient + Diffuse + Specular) * TorusCol;
}

void CalcDirectionalLight(out vec3 result, vec3 lightDir, vec3 TorusCol, vec3 Normal){
    vec3 LightColour = vec3(1.0f);
    // ambient
    float ambientStrength = 0.01;
    vec3 ambient = ambientStrength * LightColour;

    //diffuse
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * LightColour;

    // specular
    float specularStrength = 0.1f;
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);

    vec3 specular = specularStrength * spec * LightColour; 


    result = (ambient + diffuse + specular) * TorusCol;
}

void main()
{
    vec3 SumResult = vec3(0.0f);

    vec3 Normal;
    CalcTorusNormal(Normal, BefFrag, Rotation);

    
    for(int i = 0; i < Num_PointLights * 6; i += 6){
        vec3 LightPos = vec3(PointLight_Values[i], PointLight_Values[i + 1], PointLight_Values[i + 2]);
	    vec3 Terms = vec3(PointLight_Values[i + 3], PointLight_Values[i + 4], PointLight_Values[i + 5]);

        vec3 Result;
        CalcPointLight(Result, LightPos, Terms, TorusCol, Normal);
        SumResult += Result;
    }

    for(int i = 0; i < Num_DirectionalLights * 3; i+=3){
        vec3 LightDir = normalize(vec3(DirectionalLight_Values[i], DirectionalLight_Values[i + 1], DirectionalLight_Values[i + 2]));
	    
        vec3 Result;
        CalcDirectionalLight(Result, LightDir, TorusCol, Normal);
        SumResult += Result;
    }

    SumResult = SumResult / (SumResult + vec3(1.0));
  	
    FragColor = vec4(SumResult, 1.0);
}