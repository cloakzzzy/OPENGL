#include <vector>
#include <glew.h>
#include "Sphere.hpp"
#include "Flatshapes.hpp"
#include "EngineClass.hpp"
#include "Camera.hpp"
#include "EntityTemplates.hpp"
#include "Primitives.hpp"
#include "Lights.hpp"
#include <bit>
#include "Colours.hpp"

//#include "stb_image.hpp"

void Engine::Entity::Sphere::GenerateModel(int acc) {
	std::vector<float> vert;
	float cx = 0.0f;
	float cy = 0.0f;
	float cz = 0.0f;
	float r = 1.0f;
	uint32_t j = 0;
	int layers = int(ceil(float(acc) / 2));

	float th = 360.0f / float(acc);
	for (int i = 0; i < layers; i++) {
		vector<float> a = Ngonyz(cx, cy, cz, cx, cy + r, cz, th, i);
		float rada = abs(cz - a[2]);
		for (int j = 0; j < acc; j++) {
			vector<float> pa = Ngonxz(cx, a[1], cz, cx, a[1], cz + rada, th, j);
			vert.push_back(pa[0]); vert.push_back(pa[1]); vert.push_back(pa[2]);
		}
	}

	vert.push_back(cx);
	vert.push_back(cy - r);
	vert.push_back(cz);


	for (int32_t i = 0; i < vert.size(); i += 3) {
		VertexData.push_back(vert[i + 2]); // 3
		VertexData.push_back(vert[i + 1]); //2 
		VertexData.push_back(vert[i]); // 1

		float layer = floor((i / 3) / acc);
		float v = (i / 3) % (int)acc;
		
		VertexData.push_back(v / acc);
		VertexData.push_back(layer / layers);

		//std::cout << v / acc << " " << layer / layers << '\n';
	}
	
	int size = vert.size() / 3;

	for (int i = 0; i < size - 1; i++) {
		if (i > (size - acc * 1) - 1 and (i % acc != 0)) {

			if (i + 1 != size - 1) {
				
				IndicesData.push_back(size - 1);
				IndicesData.push_back(i + 1);
				IndicesData.push_back(i); 
				
				
				
			}

		}
		else {
			
			IndicesData.push_back(i); // 1
			IndicesData.push_back(i + acc); // 3
			IndicesData.push_back(i + 1); // 2
			
			
			IndicesData.push_back(i + 1);
			IndicesData.push_back(i + acc);

			if ((i + acc + 1) != size) {
				IndicesData.push_back(i + acc + 1);
			}
			else {
				IndicesData.push_back(size - 1);
			}
		}
	}

}

void Engine::Entity::Sphere::CreateBuffers() {

	GPU_ElementBuffer.CreateBuffer(IndicesData.size() * sizeof(float));
	GPU_ElementBuffer.SetData(IndicesData);

	GPU_VertexBuffer.CreateBuffer(VertexData.size() * sizeof(float), std::vector<std::pair<unsigned char, uint32_t>>{
		{OpenGLType::Vec3, 0},
		{OpenGLType::Vec2, 13}
	});

	GPU_VertexBuffer.SetData(VertexData);

	GPU_InstanceBuffer.CreateBuffer(4 * 400 * sizeof(float), std::vector<std::pair<unsigned char, uint32_t>>{
		{OpenGLType::Vec3, 1},
		{OpenGLType::Float, 2 },
		{OpenGLType::Float, 3}
	});


	// load and create a texture B
   // -------------------------
	/*
	uint32_t texture1
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	*/
	

}

void Engine::Entity::Sphere::Initialize() {
	GenerateModel(30);
	CreateBuffers();
	PrimitiveShader.SetFiles("sphere.vert", "sphere.frag");
	DepthShader.SetFiles("depthshader_sphere.vert", "depthshader.frag");
	PrimitiveShader.Use();
}
Engine::Entity::Sphere::Sphere(float pos_x, float pos_y, float pos_z, float radius, uint8_t red, uint8_t green, uint8_t blue) {
	float color = std::bit_cast<float>(((uint32_t)red) | ((uint32_t)green << 8) | ((uint32_t)blue << 16) | (0 << 24));
	
	Entity_::Generate_ID<Sphere>(ID, Index);


	this->pos_x = pos_x;
	this->pos_y = pos_y;
	this->pos_z = pos_z;

	this->radius = radius;

	this->red = red;
	this->green = green;
	this->blue = blue;

	

	
}

void Engine::Entity::Sphere::Delete() {Entity_::DataBuffer_Delete<Sphere>(ID, Index);}

