#include "IndexBuffer.h"

#include<glad/glad.h>


IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count)
	: m_Count(count)
{
	glCreateBuffers(1, &m_ID);
	glNamedBufferData(m_ID, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
	Delete();
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
	: m_ID(other.m_ID), m_Count(other.m_Count)
{
	other.m_ID = 0;
	other.m_Count = 0;
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
{
	if (this != &other)
	{
		Delete();
		m_ID = other.m_ID;
		m_Count = other.m_Count;

		other.m_ID = 0;
		other.m_Count = 0;
	}
	return *this;
}


void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::UnBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Delete()
{
	if (m_ID)
	{
		glDeleteBuffers(1, &m_ID);
		m_ID = 0;
		m_Count = 0;
	}
}
