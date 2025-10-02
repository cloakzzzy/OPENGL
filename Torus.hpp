#pragma once
#include<vector>
#include"Flatshapes.hpp"
#include <glew.h>
#include<math.h>
#include "Shader.hpp"
#include "Camera.hpp"

#define RAD 3.14159/180

using namespace std;

class torus_buffer {
	static vector<float> cs;

	static float* instance_buffer;
	static unsigned int IBO;
	static float theta;
	static vector<float> vert;
	static vector<unsigned int> ind;
	static Shader torus_shader;
	static unsigned int VBO;
	static unsigned int EBO;
	static unsigned int VAO;
	static int Size;
	

	static float dst(float x1, float y1, float x2, float y2) {
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}
	static float dst(float x1, float y1, float z1, float x2, float y2, float z2) {
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
	}
	static void generate_model(int acc) {

		vector<float> verta;
		float th = 360.0f / float(acc);
		theta = th;
		float cx = 0.f;
		float cy = 0.f;
		float cz = 0.f;
		float r = 0.f;
		float thick = 1.f;

		for (int i = 0; i < acc; i++) {
			vector<float> cb = Ngonxz(
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
	static void create_IBO(int size) {

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ARRAY_BUFFER, IBO);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), __nullptr, GL_STATIC_DRAW);

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

public:

	torus_buffer() = delete;              
	torus_buffer(const torus_buffer&) = delete;       
	torus_buffer& operator=(const torus_buffer&) = delete; 

	static float* create(int size, int acc) {
		instance_buffer = new float[size];
		Size = size;

		generate_model(acc);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), &vert.front(), GL_STATIC_DRAW);
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

		create_IBO(300 * 11);

		torus_shader.SetFiles("torus.vert", "default.frag");

		return instance_buffer;
	}

	static void render(Camera& cam) {
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBindBuffer(GL_ARRAY_BUFFER, IBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, Size * sizeof(float), instance_buffer);
		torus_shader.Use();
		torus_shader.SetMat4("projection", glm::value_ptr(cam.GetProjection()));
		torus_shader.SetMat4("view", glm::value_ptr(cam.GetView()));
		torus_shader.SetFloat("theta", theta);
		glDrawElementsInstanced(GL_TRIANGLES, ind.size(), GL_UNSIGNED_INT, 0, tor::num + 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


};



	
