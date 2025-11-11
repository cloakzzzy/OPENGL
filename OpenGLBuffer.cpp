#include "OpenGLBuffer.hpp"

void OpenGLBuffer::CreateBuffer(unsigned int BufferType, bool IsInstanceBuffer, 
	unsigned int SizeInBytes, std::vector<std::pair<unsigned char, unsigned int>> BufferLayout) {
	this->BufferType = BufferType;

	for (auto Element : BufferLayout) VertexSize += Element.first;

	glGenBuffers(1, &BufferID);
	glBindBuffer(BufferType, BufferID);
	glBufferData(BufferType, SizeInBytes, __nullptr, GL_DYNAMIC_DRAW);


	for (unsigned int ElementNum = 0; ElementNum < BufferLayout.size(); ElementNum++) {
		switch (BufferLayout[ElementNum].first) {
		case(Float):
			glEnableVertexAttribArray(BufferLayout[ElementNum].second);
			glVertexAttribPointer(BufferLayout[ElementNum].second, 1, GL_FLOAT, GL_FALSE, VertexSize, (void*)(ElementOffset));
			glVertexAttribDivisor(BufferLayout[ElementNum].second, IsInstanceBuffer);
			ElementOffset += Float;
			break;
		case(Vec2):
			glEnableVertexAttribArray(BufferLayout[ElementNum].second);
			glVertexAttribPointer(BufferLayout[ElementNum].second, 2, GL_FLOAT, GL_FALSE, VertexSize, (void*)(ElementOffset));
			glVertexAttribDivisor(BufferLayout[ElementNum].second, IsInstanceBuffer);
			ElementOffset += Vec2;
			break;
		case(Vec3):
			glEnableVertexAttribArray(BufferLayout[ElementNum].second);
			glVertexAttribPointer(BufferLayout[ElementNum].second, 3, GL_FLOAT, GL_FALSE, VertexSize, (void*)(ElementOffset));
			glVertexAttribDivisor(BufferLayout[ElementNum].second, IsInstanceBuffer);
			ElementOffset += Vec3;
			break;
		case(Vec4):
			glEnableVertexAttribArray(BufferLayout[ElementNum].second);
			glVertexAttribPointer(BufferLayout[ElementNum].second, 4, GL_FLOAT, GL_FALSE, VertexSize, (void*)(ElementOffset));
			glVertexAttribDivisor(BufferLayout[ElementNum].second, IsInstanceBuffer);
			ElementOffset += Vec4;
			break;
		}
	}
	glBindBuffer(BufferType, 0);
}
