#version 330 core
out vec4 FragColor;

in vec3 FragPos;

uniform vec3 cam_pos;
uniform vec3 col1;
uniform vec3 col2;
uniform sampler2D shadowMap;

in vec4 FragPosLightSpace;

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
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main()
{

	FragColor = vec4(ShadowCalculation(FragPosLightSpace));
	return;
	float step = 1.0f;
	
	float distance = length(cam_pos.xz - FragPos.xz);

	float fog = 1.0f;

	

	float cellx = ceil(FragPos.x / step);
	float cellz = ceil(FragPos.z / step);


	if((mod(cellx, 2.0f) == 0 && mod(cellz, 2.0f) == 1) || (mod(cellx + 1, 2.0f) == 0 && mod(cellz + 1, 2.0f) == 1)){
		FragColor = vec4(col1 * fog,1.0f);
	}
	else{
		FragColor = vec4(col2 * fog,1.0f);
	}
	
}