#version 430 core
out vec4 FragColor;

in vec3 TorusCol;

in vec3 FragPos;
in vec3 BefFrag;
in vec3 TorusPos;
in vec3 Rotation;

uniform sampler2D shadowMap;

in vec4 FragPosLightSpace;

layout(std430, binding = 2) buffer CameraData {
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

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 lightDir = normalize(vec3(0.707 * 10.f) - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.01);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 12.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
   if(projCoords.z > 1.0)
        shadow = 0.0f;
        
    return shadow;
}

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
    vec3 ViewDir = normalize(CameraPosition - FragPos);
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

void CalcDirectionalLight(out vec3 result, vec3 lightDir, vec3 TorusCol, vec3 Normal, float Shadow){
    vec3 LightColour = vec3(1.0f);
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * LightColour;



    //diffuse
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * LightColour;

    // specular
    float specularStrength = 0.1f;
    vec3 viewDir = normalize(CameraPosition - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);  
    
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);

    vec3 specular = specularStrength * spec * LightColour; 


    result = (ambient + (1.0 - Shadow) * (diffuse + specular)) * TorusCol;    
   
}

void main()
{
    vec3 SumResult = vec3(0.0f);

    vec3 Normal;
    CalcTorusNormal(Normal, BefFrag, Rotation);

    float shadow = ShadowCalculation(FragPosLightSpace, normalize(Normal));

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
        CalcDirectionalLight(Result, LightDir, TorusCol, Normal, shadow );
        SumResult += Result;
    }

    SumResult = SumResult / (SumResult + vec3(1.0));
  	
    FragColor = vec4(SumResult, 1.0);
}