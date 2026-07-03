#include"VertexBuffer.h"
#include<glad/glad.h>
#include <cassert>


VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
{
	SetData(vertices, size);
}

void VertexBuffer::SetData(float* vertices, uint32_t size)
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	m_Initialized = true;
}

void VertexBuffer::Bind() const
{
	assert(m_Initialized && "VertexBuffer not initialized. Call SetData() first.");
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::UnBind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Delete()
{
	glDeleteBuffers(1, &m_ID);
}

