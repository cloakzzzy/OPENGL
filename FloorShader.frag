/*
        //FragColor = vec4(col1 * (1.0f - shadow) / 2.0f,1.0f);
        float shadow;
        vec3 SumResult  = vec3(0.f);
        for(int i = 0; i < Num_DirectionalLights * 3; i+=3){
            
            for (int j = 0; j < 2; j++){
                shadow = ShadowCalculation(FragPosLightSpace[j], LightDir, shadowMaps[j]);
                if (shadow > 0.f){
                    break;
                }
            }

            vec3 Result;
            CalcDirectionalLight(Result, LightDir, col1, vec3(0.f, 1.0f, 0.f), shadow);
            SumResult += Result;

        }
        
        SumResult = SumResult / (SumResult + vec3(1.0));
  	
        FragColor = vec4(SumResult, 1.0);
   */

#version 430 core
out vec4 FragColor;

in vec3 FragPos;

uniform vec3 cam_pos;
uniform vec3 col1;
uniform vec3 col2;
uniform sampler2D shadowMaps[3];

in vec4 FragPosLightSpace[3];

//vec4 lightPos = vec4(0.707 * 10);

layout(std430, binding = 1) buffer Data_DirectionalLight {
    float DirectionalLight_Values[];
};

layout(std430, binding = 2) buffer GlobalUniforms {
    vec3 CameraPosition;
    mat4 CameraProjection;
    mat4 CameraView;

    uint Num_DirectionalLights;
    uint Num_PointLights;
};

float ShadowCalculation(vec4 fragPosLightSpace, vec3 LightDir, sampler2D shadowMap)
{

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 

   // return closestDepth;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(vec3(0.f, 1.0f, 0.f));
    vec3 lightDir = normalize(normalize(LightDir) * 35.f - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.001);
    
    
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
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0f;

    return shadow;
    
    /*
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

       // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0f;

    return shadow;
    */
}

void CalcDirectionalLight(out vec3 result, vec3 lightDir, vec3 TorusCol, vec3 Normal, float Shadow){
    vec3 LightColour = vec3(1.0f);
    // ambient
    float ambientStrength = 0.3;
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


    result = (ambient + (1.0 - Shadow) * (diffuse + specular)) * TorusCol;    
}


void main()
{
    float step = 1.0f;
	
	float distance = length(cam_pos.xz - FragPos.xz);

	float fog = 1.0f;

    vec3 LightDir = vec3(0.3, 1.0f, 0.3f);

	
    
    //return;
    
	float cellx = ceil(FragPos.x / step);
	float cellz = ceil(FragPos.z / step);


	if((mod(cellx, 2.0f) == 0 && mod(cellz, 2.0f) == 1) || (mod(cellx + 1, 2.0f) == 0 && mod(cellz + 1, 2.0f) == 1)){
	vec3 SumResult  = vec3(0.f);
        for(int i = 0; i < Num_DirectionalLights; i++){
            vec3 Result;
            vec3 LightDir = normalize(vec3(DirectionalLight_Values[i * 3], DirectionalLight_Values[i * 3 + 1], DirectionalLight_Values[i * 3 + 2]));
      
            float shadow = ShadowCalculation(FragPosLightSpace[i], LightDir, shadowMaps[i]);

            if(shadow == 1.0f){
                CalcDirectionalLight(Result, LightDir, col1, vec3(0.f, 1.0f, 0.f), shadow);
                Result *= Num_DirectionalLights;
                Result = Result / (Result + vec3(1.0));
                FragColor = vec4(Result, 1.0f);
                return;
            }
            if(shadow == 0.f){
                CalcDirectionalLight(Result, LightDir, col1, vec3(0.f, 1.0f, 0.f), shadow);
                SumResult += Result;
                continue;
            }
            CalcDirectionalLight(Result, LightDir, col1, vec3(0.f, 1.0f, 0.f), shadow);
            SumResult += Result;
        
        }

        SumResult = SumResult / (SumResult + vec3(1.0));

        FragColor = vec4(SumResult, 1.0);
    }
	else{
		vec3 SumResult  = vec3(0.f);
        for(int i = 0; i < Num_DirectionalLights; i++){
            vec3 Result;
            vec3 LightDir = normalize(vec3(DirectionalLight_Values[i * 3], DirectionalLight_Values[i * 3 + 1], DirectionalLight_Values[i * 3 + 2]));
      
            float shadow = ShadowCalculation(FragPosLightSpace[i], LightDir, shadowMaps[i]);

            if(shadow == 1.0f){
                CalcDirectionalLight(Result, LightDir, col2, vec3(0.f, 1.0f, 0.f), shadow);
                Result *= Num_DirectionalLights;
                Result = Result / (Result + vec3(1.0));
                FragColor = vec4(Result, 1.0f);
                return;
            }
            if(shadow == 0.f){
                CalcDirectionalLight(Result, LightDir, col2, vec3(0.f, 1.0f, 0.f), shadow);
                SumResult += Result;
                continue;
            }
            CalcDirectionalLight(Result, LightDir, col2, vec3(0.f, 1.0f, 0.f), shadow);
            SumResult += Result;
        
        }

        SumResult = SumResult / (SumResult + vec3(1.0));

        FragColor = vec4(SumResult, 1.0);
	}

    
	
	
}