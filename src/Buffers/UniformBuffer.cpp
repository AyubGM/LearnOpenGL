#include "UniformBuffer.h"
#include <glad/glad.h>

UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW); 
}

UniformBuffer::~UniformBuffer()
{
}

void UniformBuffer::SetData(uint32_t offset, uint32_t size, const void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_ID);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}
