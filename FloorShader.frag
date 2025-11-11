#version 330 core
out vec4 FragColor;

in vec3 FragPos;

void main()
{
	float step = 1.0f;

	vec3 col1 = vec3(0.6f);
	vec3 col2 = vec3(0.8f);

	float cellx = ceil(FragPos.x / step);
	float cellz = ceil(FragPos.z / step);


	if((mod(cellx, 2.0f) == 0 && mod(cellz, 2.0f) == 1) || (mod(cellx + 1, 2.0f) == 0 && mod(cellz + 1, 2.0f) == 1)){
		FragColor = vec4(col1,1.0f);
	}
	else{
		FragColor = vec4(col2,1.0f);
	}
	
}