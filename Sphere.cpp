#include <vector>
#include <glew.h>
#include "Sphere.hpp"
#include "Flatshapes.hpp"
#include "EngineClass.hpp"
#include "Camera.hpp"
#include "EntityTemplates.hpp"
#include "Primitives.hpp"
#include "Lights.hpp"

//#include "stb_image.hpp"

void Engine::Entity::Sphere::GenerateModel(int acc) {
	std::vector<float> vert;
	float cx = 0.0f;
	float cy = 0.0f;
	float cz = 0.0f;
	float r = 1.0f;
	unsigned int j = 0;
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


	for (int i = 0; i < vert.size(); i += 3) {
		VertexData.push_back(vert[i]);
		VertexData.push_back(vert[i + 1]);
		VertexData.push_back(vert[i + 2]);
		
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
				IndicesData.push_back(i);
				IndicesData.push_back(i + 1);
				IndicesData.push_back(size - 1);
			}

		}
		else {

			IndicesData.push_back(i);
			IndicesData.push_back(i + 1);
			IndicesData.push_back(i + acc);
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

	GPU_VertexBuffer.CreateBuffer(VertexData.size() * sizeof(float), std::vector<std::pair<unsigned char, unsigned int>>{
		{OpenGLType::Vec3, 0},
		{OpenGLType::Vec2, 13}
	});

	GPU_VertexBuffer.SetData(VertexData);

	GPU_InstanceBuffer.CreateBuffer(7 * 300 * sizeof(float), std::vector<std::pair<unsigned char, unsigned int>>{
		{OpenGLType::Vec3, 1},
		{OpenGLType::Float, 2 },
		{OpenGLType::Vec3, 3 }
	});



	// load and create a texture B
   // -------------------------
	/*
	unsigned int texture1
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

	GenerateModel(150);
	CreateBuffers();
	PrimitiveShader.SetFiles("sphere.vert", "sphere.frag");

	PrimitiveShader.Use();
}

Engine::Entity::Sphere::Sphere(float pos_x, float pos_y, float pos_z, float radius, float red, float green, float blue) {
	vector<float> sphere{
	pos_x,pos_y,pos_z,
	radius,
	red,green,blue,
	};

	Entity_::DataBuffer_Add<Sphere>(sphere, ID, Index);


	this->pos_x.Set(0, this, pos_x);
	this->pos_y.Set(1, this, pos_y);
	this->pos_z.Set(2, this, pos_z);

	this->radius.Set(3, this, radius);

	this->red.Set(4, this, red);
	this->green.Set(5, this, green);
	this->blue.Set(6, this, blue);

}

void Engine::Entity::Sphere::Delete() {
	Entity_::DataBuffer_Delete<Sphere>(ID, Index);
}

