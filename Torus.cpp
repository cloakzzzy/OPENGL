#include "Torus.hpp"
#include <vector>
#include "Flatshapes.hpp"
#include <glew.h>
#include "EngineClass.hpp"
#include "Camera.hpp"

void Engine::Entity::Torus::GenerateModel(int acc) {
		std::vector<float> verta;
		float th = 360.0f / float(acc);
		theta = th;
		float cx = 0.f;
		float cy = 0.f;
		float cz = 0.f;
		float r = 0.f;
		float thick = 1.f;

		for (int i = 0; i < acc; i++) {
			std::vector<float> cb = Ngonxz(
				cx, cy, cz,
				cx, cy, cz + r,
				th, i);

			cs.push_back(cb[0]); cs.push_back(cb[1]); cs.push_back(cb[2]);

			for (int j = 0; j < acc; j++) {
				vector<float> a = Ngonyz(cb[0], cb[1], cb[2], cb[0], cb[1], cb[2] + thick, th, j);
				//rotation
				a = Ngonxz(cb[0], a[1], cb[2], a[0], a[1], a[2], th, i);
				verta.push_back(a[0]); verta.push_back(a[1]); verta.push_back(a[2]);
			}
		}


		int layer;
		int quad;
		float thickness = 0.f;
		float rad = 0.f;
		float theta = 360.0f / acc;

		for (int i = 0; i < verta.size(); i += 3) {
			layer = floor(i / acc / 3.0f);



		}

		int size = verta.size() / 3;


		for (int i = 0; i < verta.size(); i += 3) {
			float layer = floor(i / acc / 3.0f);
			vert.push_back(verta[i]); vert.push_back(verta[i + 1]); vert.push_back(verta[i + 2]);

			vert.push_back(cs[layer * 3]);
			vert.push_back(layer);
			vert.push_back(cs[layer * 3 + 2]);
		}

		int h = 0;
		int start;
		int b;
		bool first = true;
		float divide = 1.0f / acc;

		for (int i = 0; i < size - (acc * (acc - acc)); i++) {
			if (i > ((size - acc)) - 1) {
				if (first == true) {
					b = i;
					first = false;
				}
				if (h == acc - 1) {
					ind.push_back(start + 1);
					ind.push_back(acc - 1);
					ind.push_back(0);

					ind.push_back(start + 1);
					ind.push_back(b);
					ind.push_back(0);

					break;
				}
				start = i;
				ind.push_back(i);
				ind.push_back(h);
				ind.push_back(h + 1);

				ind.push_back(i);
				ind.push_back(i + 1);
				ind.push_back(h + 1);

				h++;
			}

			else {
				if ((i + 1) % acc != 0) {
					ind.push_back(i + acc);
					ind.push_back(i + acc + 1);
					ind.push_back(i + 1);

					ind.push_back(i + acc);
					ind.push_back(i);
					ind.push_back(i + 1);

					if ((i + 2) % acc == 0) {
						ind.push_back(acc * floor((float)i * divide));
						ind.push_back(acc * ceil((float)i * divide));
						ind.push_back(i + acc + 1);

						ind.push_back(i + 1);
						ind.push_back(i + acc + 1);
						ind.push_back(acc * floor((float)i * divide));
					}
				}
			}
		}
	}
void Engine::Entity::Torus::CreateBuffers(){
		glGenVertexArrays(1, &Engine::u_VAO);
		glBindVertexArray(Engine::u_VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert.front(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind.size() * sizeof(float), &ind.front(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(5);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		////////////////////////// Instance Buffer
		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ARRAY_BUFFER, IBO);
		glBufferData(GL_ARRAY_BUFFER, 300 * 11 * sizeof(float), __nullptr, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, IBO);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glVertexAttribDivisor(1, 1);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribDivisor(2, 1);


		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
		glVertexAttribDivisor(3, 1);


		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
		glVertexAttribDivisor(4, 1);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

void Engine::Entity::Torus::Initialize() {
	GenerateModel(150);
	CreateBuffers();
	TorusShader.SetFiles("torus.vert", "default.frag");
	TorusShader.Use();
	TorusShader.SetFloat("theta", theta);
}

Engine::Entity::Torus::Torus(float pos_x, float pos_y, float pos_z,
	float radius, float thickness,
	float red, float green, float blue,
	float rotx, float roty, float rotz) {

	vector<float> torus{
	pos_x,pos_y,pos_z,
	radius, thickness,
	red,green,blue,
	rotx,roty,rotz
	};


	if (ObjectIDs.size() == 0) { ID = 1; }
	else { ID = ObjectIDs.back() + 1; }

	ObjectIDs.push_back(ID);

	//Inserts the torus vector to the end of the InstanceBuffer
	InstanceBuffer.insert(InstanceBuffer.end(), torus.begin(), torus.end());

	//The objects Index is the end index
	Index = ObjectIDs.size() - 1;



	this->pos_x.Set(0, this, pos_x);
	this->pos_y.Set(1, this, pos_y);
	this->pos_z.Set(2, this, pos_z);

	this->radius.Set(3, this, radius);
	this->thickness.Set(4, this, thickness);

	this->red.Set(5, this, red);
	this->green.Set(6, this, green);
	this->blue.Set(7, this, blue);

	this->rot_x.Set(8, this, rotx);
	this->rot_y.Set(9, this, roty);
	this->rot_z.Set(10, this, rotz);

}

void Engine::Entity::Torus::Delete() {
	a++;
	if (a == 1) {
		// Required in case of any, instance buffer deletions
		if (Index >= ObjectIDs.size() or ObjectIDs[Index] != ID) {
			Index = TorusBinarySearch(ObjectIDs, ID);

		}

		//Removes object id from Objectid array;
		ObjectIDs.erase(ObjectIDs.begin() + Index);
		std::cout << "Delete" << '\n';

		//removes info from instance buffer, stop rendering the torus.
		InstanceBuffer.erase(InstanceBuffer.begin() + Index * 11, InstanceBuffer.begin() + Index * 11 + 11);

	}
}

void Engine::Entity::Torus::Render(Camera& cam) {
	unsigned int NumInstances = ObjectIDs.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	//puts instance data into IBO
	glBindBuffer(GL_ARRAY_BUFFER, IBO);
	glBufferData(GL_ARRAY_BUFFER, InstanceBuffer.size() * sizeof(float), &InstanceBuffer.front(), GL_DYNAMIC_DRAW);

	TorusShader.Use();
	TorusShader.SetMat4("view", glm::value_ptr(cam.GetView()));
	TorusShader.SetMat4("projection", glm::value_ptr(cam.GetProjection()));

	glDrawElementsInstanced(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0, NumInstances);

	//std::cout << ind.size << " " << NumInstances << '\n';
}


