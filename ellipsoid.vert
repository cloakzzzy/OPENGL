#version 430 core

layout (location = 0) in vec3 a_VertexPos;

out vec3 FragCol;

layout(std430, binding = 2) buffer CameraData {
    vec3 CameraPosition;
    mat4 CameraProjection;
    mat4 CameraView;

    uint Num_DirectionalLights;
    uint Num_PointLights;
};


void main(){

   vec3 t_VertexPos = a_VertexPos;
   
   vec3 a = vec3(0.0f,0.0f,5.0f);
   vec3 b = vec3(0.0f,0.0f,-5.0f);
   float p = 15.36;
   
   float r = sqrt(  (p/2.0f) * (p/2.0f) - (length(a-b)/2.0f) * (length(a-b)/2.0f)  );

   //t_VertexPos *= 5.8198f;
   t_VertexPos*=r;

   float a7 = (t_VertexPos.y - a.y)*(t_VertexPos.y - a.y);
   float a8 = (t_VertexPos.y - b.y)*(t_VertexPos.y - b.y);

   float a1 = a.z*a.z + a7 - b.z*b.z - a8 + a.x*a.x - b.x*b.x;
   
   float a3 = -2.0f*a.z - 2.0f*b.z;
   float a4 = -2.0f*a.x + 2.0f*b.x;
   float a5 = -2.0f*a.z + 2.0f * b.z;
   float a6 = a.z*a.z + a7 - b.z*b.z - a8 + a.x*a.x - b.x*b.x;

   float A = 4.0f*a4*a4*a5*a5 - 4.0f*(a5*a5 - 4.0f*p*p)*(a4*a4 - 4.0f*p*p);
   float B = 4.0f*a4*a5*(2*a5*a1 - 2*p*p*a3) - 4.0f*(a5*a5-4.0f*p*p)*(2.0f*a4*a1 + 4.0f*p*p*a.x + 4.0f*p*p*b.x);
   float C = pow((2.0f*a5*a6 + 4.0f*p*p*a.z + 4.0f*p*p*b.z),2.0f) + (-4.0f*a5*a5 + 16.0f*p*p)*(-2.0f*p*p*(a.x*a.x + b.z*b.z + a8 + a7 + a.z*a.z + b.x*b.x) + pow(p,4.0f) + a6*a6);
   float D = -2.0f*a5*a6 + 2.0f*p*p*a3 - 2.0f*a4*a5*t_VertexPos.x;
   float E = 2.0f*a5*a5 - 8.0f*p*p;

   float disc = A*pow(t_VertexPos.x,2) + B*t_VertexPos.x + C;

      
   if (t_VertexPos.z>=0.0f){
   t_VertexPos.z = (D-sqrt(disc)) / E;
       FragCol = vec3(1.0f, 0.f, 0.f);
   }
   else{
     t_VertexPos.z = (D+sqrt(disc)) / E;
       FragCol = vec3(0.f, 1.0f, 0.f);
   }
   

   gl_Position = CameraProjection * CameraView * vec4(t_VertexPos, 1.0f);
}