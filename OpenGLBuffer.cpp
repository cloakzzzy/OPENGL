#include "OpenGLBuffer.hpp"
#include <iostream>

void OpenGLBuffer::_CreateBuffer(uint32_t BufferType ,bool IsInstanceBuffer, 
	uint32_t SizeInBytes, std::vector<std::pair<unsigned char, uint32_t>> BufferStructure) {
	this->BufferType = BufferType;

	glGenBuffers(1, &BufferID);
	glBindBuffer(BufferType, BufferID);
	glBufferData(BufferType, SizeInBytes, __nullptr, GL_STATIC_DRAW);

	if (BufferType == GL_ELEMENT_ARRAY_BUFFER) return;

	for (auto Element : BufferStructure) {
		VertexSize += Element.first;
	}

	for (uint32_t ElementNum = 0; ElementNum < BufferStructure.size(); ElementNum++) {
		switch (BufferStructure[ElementNum].first) {
		case(OpenGLType::Float):
			glEnableVertexAttribArray(BufferStructure[ElementNum].second);
			glVertexAttribPointer(BufferStructure[ElementNum].second, 1, GL_FLOAT, GL_FALSE, VertexSize, (void*)(ElementOffset));
			glVertexAttribDivisor(BufferStructure[ElementNum].second, IsInstanceBuffer);
			ElementOffset += OpenGLType::Float;
			break;
		case(OpenGLType::Vec2):
			glEnableVertexAttribArray(BufferStructure[ElementNum].second);
			glVertexAttribPointer(BufferStructure[ElementNum].second, 2, GL_FLOAT, GL_FALSE, VertexSize, (void*)(ElementOffset));
			glVertexAttribDivisor(BufferStructure[ElementNum].second, IsInstanceBuffer);
			ElementOffset += OpenGLType::Vec2;
			break;
		case(OpenGLType::Vec3):
			glEnableVertexAttribArray(BufferStructure[ElementNum].second);
			glVertexAttribPointer(BufferStructure[ElementNum].second, 3, GL_FLOAT, GL_FALSE, VertexSize, (void*)(ElementOffset));
			glVertexAttribDivisor(BufferStructure[ElementNum].second, IsInstanceBuffer);
			ElementOffset += OpenGLType::Vec3;
			break;
		case(OpenGLType::Vec4):
			glEnableVertexAttribArray(BufferStructure[ElementNum].second);
			glVertexAttribPointer(BufferStructure[ElementNum].second, 4, GL_FLOAT, GL_FALSE, VertexSize, (void*)(ElementOffset));
			glVertexAttribDivisor(BufferStructure[ElementNum].second, IsInstanceBuffer);
			ElementOffset += OpenGLType::Vec4;
			break;
		
		}
	}
	glBindBuffer(BufferType, 0);
}
