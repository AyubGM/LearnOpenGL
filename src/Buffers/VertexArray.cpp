#include "VertexArray.h"
#include <glad/glad.h>
#include <cassert>


static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    return GL_FLOAT;
	case ShaderDataType::Float2:   return GL_FLOAT;
	case ShaderDataType::Float3:   return GL_FLOAT;
	case ShaderDataType::Float4:   return GL_FLOAT;
	case ShaderDataType::Mat3:     return GL_FLOAT;
	case ShaderDataType::Mat4:     return GL_FLOAT;
	case ShaderDataType::Int:      return GL_INT;
	case ShaderDataType::Int2:     return GL_INT;
	case ShaderDataType::Int3:     return GL_INT;
	case ShaderDataType::Int4:     return GL_INT;
	case ShaderDataType::Bool:     return GL_BOOL;
	}

	std::println("Unknown ShaderDataType!");
	return 0;
}

VertexArray::VertexArray() 
{
	glCreateVertexArrays(1, &m_ID);
};

VertexArray::~VertexArray()
{
	Delete();
}

VertexArray::VertexArray(VertexArray&& other) noexcept
	: m_ID(other.m_ID), m_AttribIndex(other.m_AttribIndex)
{
	other.m_ID = 0;
    other.m_AttribIndex = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
{
	if (this != &other)
	{
		Delete();
		m_ID = other.m_ID;
        m_AttribIndex = other.m_AttribIndex;

		other.m_ID = 0;
        other.m_AttribIndex = 0;
        
	}
	return *this;
}


void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer)
{
    assert(vertexBuffer.GetLayout().GetStride() != 0 && "Vertex Buffer has no layout!");

    glBindVertexArray(m_ID);
    vertexBuffer.Bind();

    const auto& layout = vertexBuffer.GetLayout();
    for (const auto& element : layout)
    {
        switch (element.Type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
        {
            glEnableVertexAttribArray(m_AttribIndex);
            glVertexAttribPointer(m_AttribIndex,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<const void*>(element.Offset));
            glVertexAttribDivisor(m_AttribIndex, element.Divisor);
            m_AttribIndex++;
            break;
        }
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
        case ShaderDataType::Bool:
        {
            glEnableVertexAttribArray(m_AttribIndex);
            glVertexAttribIPointer(m_AttribIndex,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                layout.GetStride(),
                reinterpret_cast<const void*>(element.Offset));
            glVertexAttribDivisor(m_AttribIndex, element.Divisor);
            m_AttribIndex++;
            break;
        }
        case ShaderDataType::Mat3:
        case ShaderDataType::Mat4:
        {
            uint8_t columnCount = element.Type == ShaderDataType::Mat3 ? 3 : 4;
            for (uint8_t i = 0; i < columnCount; i++)
            {
                glEnableVertexAttribArray(m_AttribIndex);
                glVertexAttribPointer(m_AttribIndex,
                    columnCount,
                    ShaderDataTypeToOpenGLBaseType(element.Type),
                    element.Normalized ? GL_TRUE : GL_FALSE,
                    layout.GetStride(),
                    reinterpret_cast<const void*>(element.Offset + sizeof(float) * columnCount * i));
                glVertexAttribDivisor(m_AttribIndex, element.Divisor);
                m_AttribIndex++;
            }
            break;
        }
        default:
            assert(false && "Unknown ShaderDataType");
        }
    }

    vertexBuffer.UnBind();
}

void VertexArray::SetIndexBuffer(const IndexBuffer& indexBuffer)
{
	glBindVertexArray(m_ID);
	indexBuffer.Bind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}

void VertexArray::Delete()
{
	if (m_ID)
	{
		glDeleteVertexArrays(1, &m_ID);
		m_ID = 0;
	}
}