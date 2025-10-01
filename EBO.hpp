#pragma once

#include<GL/glew.h>

class EBO
{
public:
	// ID reference of Elements Buffer Object
	GLuint ID;



	void Create(vector<unsigned int> vertices) {
		glGenBuffers(1, &ID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_DYNAMIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// Binds the EBO
	void Bind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	}
	// Unbinds the EBO
	void Unbind() {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	// Deletes the EBO
	void Delete() {
		glDeleteBuffers(1, &ID);
	}
};
