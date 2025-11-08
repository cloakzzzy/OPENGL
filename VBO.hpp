#pragma once

#include<GL/glew.h>
#include <vector>

class VBO
{
public:
	GLuint ID;
	void Create(unsigned int size_in_bytes) {
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, size_in_bytes, 0, GL_STATIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void Create(std::vector<float> vertices, unsigned int size_in_bytes) {
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, size_in_bytes, &vertices.front(), GL_STATIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void Create(std::vector<float> vertices) {
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ARRAY_BUFFER, ID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_DYNAMIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Binds the VBO
	void Bind() {
		glBindBuffer(GL_ARRAY_BUFFER, ID);
	}
	// Unbinds the VBO
	void Unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	// Deletes the VBO
	void Delete() {
		glDeleteBuffers(1, &ID);
	}
};
