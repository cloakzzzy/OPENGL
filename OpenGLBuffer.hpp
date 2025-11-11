#pragma once

#include <glew.h>
#include <vector>
#include <map>

class OpenGLBuffer{
	unsigned int BufferID;
public:

	const static unsigned char Float = 1 * sizeof(float);
	const static unsigned char Vec2 = 2 * sizeof(float);
	const static unsigned char Vec3 = 3 * sizeof(float);
	const static unsigned char Vec4 = 4 * sizeof(float);

	unsigned int ElementOffset = 0;
	unsigned int VertexSize = 0;
	unsigned int BufferType;
	
	void CreateBuffer(unsigned int BufferType, bool IsInstanceBuffer, unsigned int SizeInBytes, std::vector<std::pair<unsigned char, unsigned int>> BufferLayout);

	inline void SetData(std::vector<float> &Data) {
		glBufferData(BufferType, Data.size() * sizeof(float), &Data.front(), GL_DYNAMIC_DRAW);	
	}
	inline void Bind() {
		glBindBuffer(BufferType, BufferID);
	}
	inline void Unbind() {
		glBindBuffer(BufferType, 0);
	}
};