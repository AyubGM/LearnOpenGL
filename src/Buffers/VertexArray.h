#pragma once

#include "VertexBuffer.h"

#include <stdint.h>
#include "IndexBuffer.h"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();


	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	void AddVertexBuffer(const VertexBuffer& vertexBuffer);
	void SetIndexBuffer(const IndexBuffer& indexBuffer);

	void Bind() const;
	void UnBind() const;
	void Delete();

private:
	uint32_t m_ID = 0;
	uint32_t m_AttribIndex = 0;

};
