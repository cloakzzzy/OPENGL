#include <vector>
#include <glew.h>
#include "Sphere.hpp"
#include "Flatshapes.hpp"
#include "EngineClass.hpp"
#include "Camera.hpp"

void Engine::Entity::Sphere::GenerateModel(int acc) {

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

	float recipx = 1.0f / acc;
	float recipy = 1.0f / float(ceil(acc / 2));

	for (int i = 0; i < vert.size(); i += 3) {
		Vert.push_back(vert[i]);
		Vert.push_back(vert[i + 1]);
		Vert.push_back(vert[i + 2]);
		Vert.push_back(vert[i]);
		Vert.push_back(vert[i + 1]);
	}
	int size = vert.size() / 3;

	for (int i = 0; i < size - 1; i++) {
		if (i > (size - acc * 1) - 1 and (i % acc != 0)) {

			if (i + 1 != size - 1) {
				ind.push_back(i);
				ind.push_back(i + 1);

				ind.push_back(size - 1);
			}

		}
		else {

			ind.push_back(i);
			ind.push_back(i + 1);
			ind.push_back(i + acc);
			ind.push_back(i + 1);
			ind.push_back(i + acc);

			if ((i + acc + 1) != size) {
				ind.push_back(i + acc + 1);
			}
			else {
				ind.push_back(size - 1);
			}
		}
	}

}

void Engine::Entity::Sphere::CreateBuffers() {
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vert.size() * sizeof(float), &Vert.front(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind.front(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	/*
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	glBufferData(GL_ARRAY_BUFFER, 7 * 300 * sizeof(float), __nullptr, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, IBO);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glVertexAttribDivisor(1, 1);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribDivisor(2, 1);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
	glVertexAttribDivisor(3, 1);
	*/
	IBO.CreateBuffer(GL_ARRAY_BUFFER, true, 300 * 7 * sizeof(float),
		std::vector<std::pair<unsigned char, unsigned int>>{
		{OpenGLBuffer::Vec3, 1},
		{OpenGLBuffer::Float,2},
		{OpenGLBuffer::Vec3, 3}});
	
	
}

void Engine::Entity::Sphere::Initialize() {
	GenerateModel(150);
	CreateBuffers();
	SphereShader.SetFiles("sphere.vert", "sphere.frag");
}

Engine::Entity::Sphere::Sphere(float pos_x, float pos_y, float pos_z, float radius, float red, float green, float blue) {
	vector<float> sphere{
	pos_x,pos_y,pos_z,
	radius, 
	red,green,blue,
	};


	if (ObjectIDs.size() == 0) { ID = 1; }
	else { ID = ObjectIDs.back() + 1; }

	ObjectIDs.push_back(ID);

	//Inserts the torus vector to the end of the InstanceBuffer
	InstanceBuffer.insert(InstanceBuffer.end(), sphere.begin(), sphere.end());

	//The objects Index is the end index
	Index = ObjectIDs.size() - 1;


	this->pos_x.Set(0, this, pos_x);
	this->pos_y.Set(1, this, pos_y);
	this->pos_z.Set(2, this, pos_z);

	this->radius.Set(3, this, radius);
	
	this->red.Set(4, this, red);
	this->green.Set(5, this, green);
	this->blue.Set(6, this, blue);

}

void Engine::Entity::Sphere::Delete() {
	// Required in case of any instance buffer deletions
	if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) { Index = SphereBinarySearch(ObjectIDs, ID); }

	//Required if called on deleted Object
	if (Index == 4294967295) return;

	//Removes object id from Objectid array;
	ObjectIDs.erase(ObjectIDs.begin() + Index);

	//removes info from instance buffer, stop rendering the torus.
	InstanceBuffer.erase(InstanceBuffer.begin() + Index * 7, InstanceBuffer.begin() + Index * 7 + 7);

}

void Engine::Entity::Sphere::Render(Camera& cam) {
	unsigned int NumInstances = ObjectIDs.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	
	IBO.Bind();
	IBO.SetData(InstanceBuffer);

	SphereShader.Use();
	SphereShader.SetMat4("view", glm::value_ptr(cam.GetView()));
	SphereShader.SetMat4("projection", glm::value_ptr(cam.GetProjection()));

	glDrawElementsInstanced(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0, NumInstances);


}

