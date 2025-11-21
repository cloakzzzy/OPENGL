#version 330 core
out vec4 FragColor;

in vec3 FragPos;

uniform vec3 cam_pos;
uniform vec3 col1;
uniform vec3 col2;
uniform sampler2D shadowMap;

in mat4 lsp;

in vec4 FragPosLightSpace;

vec4 lightPos = vec4(0.707 * 10);

float ShadowCalculation(vec4 fragPosLightSpace)
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
    vec3 normal = normalize(vec3(0.f, 1.0f, 0.f));
    vec3 lightDir = normalize(vec3(0.707 * 10.f) - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
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
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0f;
        
    return shadow;
}

void main()
{

	//FragColor = vec4(0.2f);
	//FragColor = vec4(LightSpaceMatrix);
	
	//vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
	//FragColor = vec4(FragPosLightSpace.xyz, 1.0f);
	//FragColor = 
	//FragColor = vec4(texture(shadowMap, FragPos.xz).r); 
	float step = 1.0f;
	
	float distance = length(cam_pos.xz - FragPos.xz);

	float fog = 1.0f;

	

	float cellx = ceil(FragPos.x / step);
	float cellz = ceil(FragPos.z / step);



	if((mod(cellx, 2.0f) == 0 && mod(cellz, 2.0f) == 1) || (mod(cellx + 1, 2.0f) == 0 && mod(cellz + 1, 2.0f) == 1)){
		FragColor = vec4(col1 * (1.0f - ShadowCalculation(FragPosLightSpace)) / 2.0f,1.0f);
	}
	else{
		FragColor = vec4(col2 * (1.0f - ShadowCalculation(FragPosLightSpace)) / 2.0f,1.0f);
	}
	
	
}