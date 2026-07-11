#include"VertexBuffer.h"
#include<glad/glad.h>
#include <cassert>


VertexBuffer::VertexBuffer(const void* vertices, uint32_t size)
{
	SetData(vertices, size);
}

VertexBuffer::~VertexBuffer() {
	Delete();
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
	: m_ID(other.m_ID), m_Layout(std::move(other.m_Layout)), m_Initialized(other.m_Initialized)
{
	other.m_ID = 0;
	other.m_Initialized = false;
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
{
	if (this != &other)
	{
		Delete(); // Clean up current resources before taking ownership
		m_ID = other.m_ID;
		m_Layout = std::move(other.m_Layout);
		m_Initialized = other.m_Initialized;

		other.m_ID = 0;
		other.m_Initialized = false;
	}
	return *this;
}

void VertexBuffer::SetData(const void* vertices, uint32_t size)
{
	if (!m_Initialized)
	{
		// OpenGL 4.5+ Direct State Access
		glCreateBuffers(1, &m_ID);
		glNamedBufferData(m_ID, size, vertices, GL_STATIC_DRAW);
		m_Initialized = true;
	}
	else
	{
		// If already created, just update the data (avoids memory leak)
		glNamedBufferSubData(m_ID, 0, size, vertices);
	}
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
	if (m_ID)
	{
		glDeleteBuffers(1, &m_ID);
		m_ID = 0;
		m_Initialized = false;
	}
}

