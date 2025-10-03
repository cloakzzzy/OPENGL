#pragma once
#include<vector>
#include"Flatshapes.hpp"
#include <glew.h>
#include<math.h>
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#define RAD 3.14159/180

using namespace std;
namespace tor {
	unsigned int instanceVBO;
	unsigned int num = 0;
	float theta;
	vector<float> vert;
	vector<unsigned int> ind;
	Shader TorusShader;
	VBO VBO1;
	EBO EBO1;
	vector<float> offs;
	unsigned int torus_max = 300;
}


vector<float> cs;

class Torus {
	float x, y, z, thickness, radius, r, g, b, rotx, roty, rotz;
	float aradius; float athickness;
	float nan = sqrt(-1.0f);
	unsigned int offset = 0;
	float dst(float x1, float y1, float x2, float y2) {
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}
	float dst(float x1, float y1, float z1, float x2, float y2, float z2) {
		return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
	}
public:
	static void GenerateModel(int acc, VAO& VAO1) {

		vector<float> verta;
		float th = 360.0f / float(acc);
		tor::theta = th;
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
			tor::vert.push_back(verta[i]); tor::vert.push_back(verta[i + 1]); tor::vert.push_back(verta[i + 2]);

			tor::vert.push_back(cs[layer * 3]);
			tor::vert.push_back(layer);
			tor::vert.push_back(cs[layer * 3 + 2]);
		}

		//cout << "size" << '\n';
		//cout << tor::vert.size() << '\n';

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
					tor::ind.push_back(start + 1);
					tor::ind.push_back(acc - 1);
					tor::ind.push_back(0);

					tor::ind.push_back(start + 1);
					tor::ind.push_back(b);
					tor::ind.push_back(0);

					break;
				}
				start = i;
				tor::ind.push_back(i);
				tor::ind.push_back(h);
				tor::ind.push_back(h + 1);

				tor::ind.push_back(i);
				tor::ind.push_back(i + 1);
				tor::ind.push_back(h + 1);

				h++;
			}

			else {
				if ((i + 1) % acc != 0) {
					tor::ind.push_back(i + acc);
					tor::ind.push_back(i + acc + 1);
					tor::ind.push_back(i + 1);

					tor::ind.push_back(i + acc);
					tor::ind.push_back(i);
					tor::ind.push_back(i + 1);

					if ((i + 2) % acc == 0) {
						tor::ind.push_back(acc * floor((float)i * divide));
						tor::ind.push_back(acc * ceil((float)i * divide));
						tor::ind.push_back(i + acc + 1);

						tor::ind.push_back(i + 1);
						tor::ind.push_back(i + acc + 1);
						tor::ind.push_back(acc * floor((float)i * divide));
					}
				}
			}
		}

		VAO1.Bind();

		tor::VBO1.Bind();
		tor::VBO1.Create(tor::vert);
		tor::VBO1.Unbind();

		tor::EBO1.Bind();
		tor::EBO1.Create(tor::ind);
		tor::EBO1.Unbind();

		VAO1.LinkVBO(tor::VBO1, 6, 0, 3, 0);
		VAO1.LinkVBO(tor::VBO1, 6, 5, 3, 3);


		tor::TorusShader.SetFiles(
			"torus.vert",
			"default.frag");

		VAO1.Unbind();
	}



	//posx, posy, posz, 
	//radius, thickness,
	//colourr, colourg, colourb, 
	//rotx, roty, rotz = 11floats

	static void CreateInstanceVBO() {

		glGenBuffers(1, &tor::instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tor::instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, 11 * tor::torus_max * sizeof(float), __nullptr, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, tor::instanceVBO);

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

	void Create(float X, float Y, float Z, float Radius, float Thickness, float R, float G, float B, float Rotx, float Roty, float Rotz) {
		aradius = Radius; athickness = Thickness;
		Thickness *= 0.5f;
		Radius -= Thickness;
		glBindBuffer(GL_ARRAY_BUFFER, tor::instanceVBO);
		x = X; y = Y; z = Z; radius = Radius; thickness = Thickness;  r = R; g = G; b = B; rotx = Rotx; roty = Roty; rotz = Rotz;
		vector<float> torus{
			x,y,z,
			radius,thickness,
			r,g,b,
			rotx, roty, rotz
		};


		if (tor::offs.size() > 0) {
			glBufferSubData(GL_ARRAY_BUFFER, tor::offs.back() * sizeof(float), torus.size() * sizeof(float), &torus.front());
			offset = tor::offs.back();
			tor::offs.pop_back();
			if (offset == (tor::num + 1) * 11.0f) {
				tor::num++;
				//cout << "replacing end sphere " << offset << '\n';
			}
			//cout << "new sphere " << offset << '\n';
		}
		else {
			cout << "in in " << tor::num << '\n';
			tor::num++;
			glBufferSubData(GL_ARRAY_BUFFER, tor::num * 11.0f * sizeof(float), torus.size() * sizeof(float), &torus.front());
			offset = tor::num * 11;
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Delete() {
		vector<float> notorus{ 0,0,0,0,0,0,0,0,0,0,0 };
		glBindBuffer(GL_ARRAY_BUFFER, tor::instanceVBO);
		glBufferSubData(GL_ARRAY_BUFFER, offset * sizeof(float), 11 * sizeof(float), &notorus.front());
		tor::offs.push_back(offset);
		if (offset == tor::num * 11.0f) {
			tor::num--;
			//cout << "end sphere hit " << offset << '\n';
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	/*
	bool RayCollision(glm::vec3 rayorigin, glm::vec3 raydir) {

		glm::vec2 raydirtwo = glm::vec2(raydir.x, raydir.z);
		glm::vec2 rayorigintwo = glm::vec2(rayorigin.x, rayorigin.z);
		vector<float> nonan;
		//outer circle
		float ao = glm::dot(raydirtwo, raydirtwo);
		float bo = 2.0f * glm::dot(rayorigintwo - glm::vec2(x,z), raydirtwo);
		float co = glm::dot(rayorigintwo - glm::vec2(x,z), rayorigintwo - glm::vec2(x, z)) - aradius * aradius;
		float discriminanto = bo * bo - 4.0f * ao * co;
		//first collision
		float tof = (-bo - sqrt(discriminanto)) / (2.0f * ao);
		float yof = rayorigin.y + tof * raydir.y;
		// if negative or out of bounds
		if ((tof < 0.f) or ((yof > y + athickness * 0.5f) or (yof < y - athickness * 0.5f))) {
			tof = nan;
		}
		//second collision
		float tos = (-bo + sqrt(discriminanto)) / (2.0f * ao);
		float yos = rayorigin.y + tos * raydir.y;
		if ((tos < 0.f) or ((yos > y + athickness * 0.5f) or (yos < y - athickness * 0.5f))) {
			tos = nan;
		}

		//==================================================================

		//inner circle
		float ai = glm::dot(raydirtwo, raydirtwo);
		float bi = 2.0f * glm::dot(rayorigintwo - glm::vec2(x, z), raydirtwo);
		float ci = glm::dot(rayorigintwo - glm::vec2(x, z), rayorigintwo - glm::vec2(x, z)) - (aradius - athickness) * (aradius - athickness);
		float discriminanti = bi * bi - 4.0f * ai * ci;
		//first collision
		float tif = (-bi - sqrt(discriminanti)) / (2.0f * ai);
		float yif = rayorigin.y + tif * raydir.y;
		// if negative or out of bounds
		if ((tif < 0.f) or ((yif > y + athickness * 0.5f) or (yif < y - athickness * 0.5f))) {
			tif = nan;
		}
		//second collision
		float tis = (-bi + sqrt(discriminanti)) / (2.0f * ai);
		float yis = rayorigin.y + tis * raydir.y;
		if ((tis < 0.f) or ((yis > y + athickness * 0.5f) or (yis < y - athickness * 0.5f))) {
			tis = nan;
		}

		//========================================================================================
		//top
		float t_t = ((y + athickness * 0.5f) - (rayorigin.y)) / raydir.y;
		glm::vec2 t_hit = rayorigintwo + raydirtwo * t_t;
		float t_distance = dst(t_hit.x, t_hit.y, x, z);
		//out of bounds
		if (!((t_distance > aradius - athickness) and (t_distance < aradius))) {
			//cout << distance << " " << aradius - athickness <<" "<< aradius<<'\n';
			t_t = nan;
		}

		float b_t = ((y - athickness * 0.5f) - (rayorigin.y)) / raydir.y;
		glm::vec2 b_hit = rayorigintwo + raydirtwo * b_t;
		float b_distance = dst(b_hit.x, b_hit.y, x, z);
		//out of bounds
		if (!((b_distance > aradius - athickness) and (b_distance < aradius))) {
			//cout << distance << " " << aradius - athickness <<" "<< aradius<<'\n';
			b_t = nan;
		}

		cout << "==============================================" << '\n';
		cout << "[OUTER]: "<<tof << " " << tos << '\n';
		cout << "[INNER]: " << tif << " " << tis << '\n';
		cout << "[TOP]: " << t_t << " " << t_distance <<" " << aradius - athickness << " " << aradius<< '\n';
		cout << "[BOTTOM]: " << b_t << " " << b_distance << " " << aradius - athickness << " " << aradius << '\n';
		cout << "==============================================" << '\n';


		//inner cannot be collided at the same time as top or bottom
		//max collisions is 4, min collisions is 2

		vector<float> ts = { tof, tos, tif, tis, t_t, b_t };
		for (int i = 0; i < ts.size(); i++) {
			if (!isnan(ts[i])) {
				nonan.push_back(ts[i]);
				cout << i << " ";
			}
		}
		cout << '\n';
		//cout << '\n';
		//left side generation
		for (float i : nonan) {
			glm::vec3 hit = rayorigin + raydir * i;
			glm::vec2 hittwo = glm::vec2(hit.x, hit.z);
			//cout << "[" << i << "] " << hit.x << " " << hit.y << " " << hit.z << '\n';
			glm::vec2 norm = glm::normalize(hittwo - glm::vec2(x, z));
			float theta = 90.0f - ((round((float(asin(norm.y) * RD)) * 100.0f)) / 100.0f);
			//cout << theta << " " << (float(asin(norm.y) * RD))<< '\n';
			int sector = floor(theta / tor::theta);


			cout << sector << " " << tor::theta <<" " << theta<< '\n';
			//cout << '\n';
		}
		cout << "=====================================" << '\n';

	}
	*/

	static void Render(Camera& cam) {
		tor::VBO1.Bind();
		tor::EBO1.Bind();
		glBindBuffer(GL_ARRAY_BUFFER, tor::instanceVBO);
		tor::TorusShader.Use();
		tor::TorusShader.SetMat4("projection", glm::value_ptr(cam.GetProjection()));
		tor::TorusShader.SetMat4("view", glm::value_ptr(cam.GetView()));
		tor::TorusShader.SetFloat("theta", tor::theta);
		glDrawElementsInstanced(GL_TRIANGLES, tor::ind.size(), GL_UNSIGNED_INT, 0, tor::num + 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	static void DeleteBuffers() {
		tor::VBO1.Delete();
		tor::EBO1.Delete();
		glDeleteBuffers(1, &tor::instanceVBO);
	}
};