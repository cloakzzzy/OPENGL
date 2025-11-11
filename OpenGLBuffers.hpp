#pragma once
#include "OpenGLBuffer.hpp"

class OpenGL_VertexBuffer : public OpenGLBuffer {
public: 
	void CreateBuffer(unsigned int SizeInBytes, std::vector<std::pair<unsigned char, unsigned int>> BufferStructure){
		_CreateBuffer(GL_ARRAY_BUFFER, false, SizeInBytes, BufferStructure);
	}
};

class OpenGL_ElementBuffer : public OpenGLBuffer {
public: 
	void CreateBuffer(unsigned int SizeInBytes) {
		_CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, false, SizeInBytes, std::vector<std::pair<unsigned char, unsigned int>>{});
	}
	
};

class OpenGL_InstanceBuffer : public OpenGLBuffer {
public:
	void CreateBuffer(unsigned int SizeInBytes, std::vector<std::pair<unsigned char, unsigned int>> BufferStructure) {
		_CreateBuffer(GL_ARRAY_BUFFER, true, SizeInBytes, BufferStructure);
	}
};