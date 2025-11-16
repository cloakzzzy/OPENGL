#pragma once

#include <glew.h>
#include <vector>
#include <map>

struct OpenGLType {
	const static unsigned char Float = 1 * sizeof(float);
	const static unsigned char Vec2 = 2 * sizeof(float);
	const static unsigned char Vec3 = 3 * sizeof(float);
	const static unsigned char Vec4 = 4 * sizeof(float);
};

class OpenGLBuffer {
	friend class OpenGL_VertexBuffer;
	friend class OpenGL_ElementBuffer;
	friend class OpenGL_InstanceBuffer;

private:
	OpenGLBuffer() {};

	unsigned int BufferID;
	void _CreateBuffer(unsigned int BufferType, bool IsInstanceBuffer, unsigned int SizeInBytes, std::vector<std::pair<unsigned char, unsigned int>> BufferStructure);

	unsigned int ElementOffset = 0;
	unsigned int VertexSize = 0;
	unsigned int BufferType;

public:
	inline void SetData(std::vector<float>& Data) {
		glBindBuffer(BufferType, BufferID);
		glBufferSubData(BufferType, 0, Data.size() * sizeof(float), &Data.front());
		glBindBuffer(BufferType, 0);
	}

	inline void SetData(std::vector<unsigned int>& Data) {
		glBindBuffer(BufferType, BufferID);
		glBufferSubData(BufferType, 0, Data.size() * sizeof(float), &Data.front());
		glBindBuffer(BufferType, 0);
	}

	inline void Bind() {
		glBindBuffer(BufferType, BufferID);
	}
	inline void Unbind() {
		glBindBuffer(BufferType, 0);
	}
	
};