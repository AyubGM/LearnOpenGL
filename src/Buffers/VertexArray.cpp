#include "VertexArray.h"
#include <glad/glad.h>


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
	glGenVertexArrays(1, &m_ID);
};

void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer) const
{
	glBindVertexArray(m_ID);
	vertexBuffer.Bind();

	uint32_t attribIndex = 0;
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
			glEnableVertexAttribArray(attribIndex);
			glVertexAttribPointer(attribIndex,
				element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(void*)element.Offset);
			attribIndex++;
			break;
		}

		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		{
			glEnableVertexAttribArray(attribIndex);
			glVertexAttribPointer(attribIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(void*)element.Offset);
			attribIndex++;
			break;
		}
		default:
			std::println("Unknown ShaderDataType!");


		}
		
	}
	
	vertexBuffer.UnBind();
};

void VertexArray::Bind()
{
	glBindVertexArray(m_ID);
};
void VertexArray::UnBind()
{
	glBindVertexArray(0);
};
void VertexArray::Delete()
{
	glDeleteVertexArrays(1, &m_ID);
};
